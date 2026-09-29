#include "infrastructure/parsing/LexborHtmlParser.h"

#include <stdexcept>

#include <QFile>
#include <QFileInfo>
#include <QStringList>

#include <lexbor/css/css.h>
#include <lexbor/html/html.h>
#include <lexbor/selectors/selectors.h>

namespace winccmcp::infrastructure::parsing {

using core::models::Document;

namespace {

constexpr const char kArticleSelector[] = "#wh_topic_body article[role='article']";
constexpr const char kShortdescSelector[] = "p.shortdesc";

// Only look for the first match (analogous to BeautifulSoup.select_one) -
// these help pages never have more than one of either element.
struct FirstMatch {
    lxb_dom_node_t* node = nullptr;
};

lxb_status_t firstMatchCallback(lxb_dom_node_t* node, lxb_css_selector_specificity_t /*spec*/, void* ctx) {
    auto* match = static_cast<FirstMatch*>(ctx);
    if (match->node == nullptr) {
        match->node = node;
    }
    return LXB_STATUS_OK;
}

// Recursively collects the text of every text node in the subtree (already
// trimmed and with empty pieces dropped), skipping the contents of
// <script>/<style> - analogous to BeautifulSoup: decompose(script/style)
// first, then get_text(sep, strip=True).
void collectText(lxb_dom_node_t* node, QStringList& out) {
    for (lxb_dom_node_t* child = node->first_child; child != nullptr; child = child->next) {
        if (child->type == LXB_DOM_NODE_TYPE_ELEMENT) {
            if (child->local_name == LXB_TAG_SCRIPT || child->local_name == LXB_TAG_STYLE) {
                continue;
            }
            collectText(child, out);
        } else if (child->type == LXB_DOM_NODE_TYPE_TEXT) {
            size_t len = 0;
            const lxb_char_t* data = lxb_dom_node_text_content(child, &len);
            const QString text = QString::fromUtf8(reinterpret_cast<const char*>(data), static_cast<int>(len)).trimmed();
            if (!text.isEmpty()) {
                out << text;
            }
        }
    }
}

// A second pass, mirroring the Python version: individual text nodes can
// themselves contain internal newlines (multi-line formatting in the source
// DITA) - collapse those the same way as ordinary blank lines.
QString normalizeLines(const QString& text) {
    QStringList lines;
    for (const QString& line : text.split(QLatin1Char('\n'))) {
        const QString trimmed = line.trimmed();
        if (!trimmed.isEmpty()) {
            lines << trimmed;
        }
    }
    return lines.join(QLatin1Char('\n'));
}

lxb_dom_node_t* findFirst(lxb_selectors_t* selectors, lxb_dom_node_t* root, lxb_css_selector_list_t* list) {
    FirstMatch match;
    lxb_selectors_find(selectors, root, list, firstMatchCallback, &match);
    return match.node;
}

} // namespace

LexborHtmlParser::LexborHtmlParser() {
    m_cssParser = lxb_css_parser_create();
    if (lxb_css_parser_init(m_cssParser, nullptr) != LXB_STATUS_OK) {
        throw std::runtime_error("Failed to initialize the lexbor CSS parser");
    }

    m_selectors = lxb_selectors_create();
    if (lxb_selectors_init(m_selectors) != LXB_STATUS_OK) {
        throw std::runtime_error("Failed to initialize lexbor selectors");
    }

    m_articleSelectorList = lxb_css_selectors_parse(
        m_cssParser, reinterpret_cast<const lxb_char_t*>(kArticleSelector), sizeof(kArticleSelector) - 1);
    if (m_articleSelectorList == nullptr || m_cssParser->status != LXB_STATUS_OK) {
        throw std::runtime_error("Failed to parse the article CSS selector");
    }

    m_shortdescSelectorList = lxb_css_selectors_parse(
        m_cssParser, reinterpret_cast<const lxb_char_t*>(kShortdescSelector), sizeof(kShortdescSelector) - 1);
    if (m_shortdescSelectorList == nullptr || m_cssParser->status != LXB_STATUS_OK) {
        throw std::runtime_error("Failed to parse the shortdesc CSS selector");
    }
}

LexborHtmlParser::~LexborHtmlParser() {
    if (m_articleSelectorList) {
        lxb_css_selector_list_destroy_memory(m_articleSelectorList);
    }
    if (m_shortdescSelectorList) {
        lxb_css_selector_list_destroy_memory(m_shortdescSelectorList);
    }
    if (m_selectors) {
        lxb_selectors_destroy(m_selectors, true);
    }
    if (m_cssParser) {
        lxb_css_parser_destroy(m_cssParser, true);
    }
}

std::optional<Document> LexborHtmlParser::parseFile(const QString& absoluteFilePath, const QString& relativeDocPath) {
    QFile file(absoluteFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to open file: " + absoluteFilePath.toStdString());
    }
    const QByteArray raw = file.readAll();

    lxb_html_document_t* document = lxb_html_document_create();
    if (document == nullptr) {
        throw std::runtime_error("lxb_html_document_create returned nullptr");
    }

    const lxb_status_t parseStatus = lxb_html_document_parse(
        document, reinterpret_cast<const lxb_char_t*>(raw.constData()), static_cast<size_t>(raw.size()));
    if (parseStatus != LXB_STATUS_OK) {
        lxb_html_document_destroy(document);
        throw std::runtime_error("Failed to parse HTML: " + absoluteFilePath.toStdString());
    }

    std::optional<Document> result;

    lxb_dom_node_t* documentNode = lxb_dom_interface_node(document);
    lxb_dom_node_t* articleNode = findFirst(m_selectors, documentNode, m_articleSelectorList);

    if (articleNode != nullptr) {
        QStringList bodyPieces;
        collectText(articleNode, bodyPieces);
        const QString text = normalizeLines(bodyPieces.join(QLatin1Char('\n')));

        if (!text.isEmpty()) {
            Document doc;
            doc.path = relativeDocPath;
            doc.text = text;

            size_t titleLen = 0;
            const lxb_char_t* titleData = lxb_html_document_title(document, &titleLen);
            doc.title = (titleData != nullptr && titleLen > 0)
                            ? QString::fromUtf8(reinterpret_cast<const char*>(titleData), static_cast<int>(titleLen)).trimmed()
                            : QFileInfo(absoluteFilePath).completeBaseName();

            lxb_dom_node_t* shortdescNode = findFirst(m_selectors, articleNode, m_shortdescSelectorList);
            if (shortdescNode != nullptr) {
                QStringList shortdescPieces;
                collectText(shortdescNode, shortdescPieces);
                doc.shortdesc = shortdescPieces.join(QLatin1Char(' '));
            }

            result = doc;
        }
    }

    lxb_html_document_destroy(document);
    return result;
}

} // namespace winccmcp::infrastructure::parsing
