#ifndef WINCCMCP_INFRASTRUCTURE_PARSING_LEXBORHTMLPARSER_H__1C2D3E4F_5AAB_4ACC_9455_ABC1D2E3F4A5__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PARSING_LEXBORHTMLPARSER_H__1C2D3E4F_5AAB_4ACC_9455_ABC1D2E3F4A5__INCLUDED_

#include "core/interfaces/IHtmlDocumentParser.h"

// Forward-declare only - lexbor stays an implementation detail, upstream
// layers' .h files don't pull in lexbor's headers.
struct lxb_css_parser;
struct lxb_selectors;
struct lxb_css_selector_list;

namespace winccmcp::infrastructure::parsing {

// IHtmlDocumentParser on top of lexbor: an HTML5 parser + CSS selectors, a
// direct analogue of `BeautifulSoup(...).select_one(...)` from the Python
// version of this project. The useful payload of a WinCC OA help page lives
// in <div id="wh_topic_body"><article role="article">...</article></div> -
// everything else (navigation, footer, scripts) is discarded.
//
// Not thread-safe: like the rest of the ingestion pipeline, only one call to
// parseFile() at a time is assumed.
class LexborHtmlParser : public core::interfaces::IHtmlDocumentParser {
public:
    LexborHtmlParser();
    ~LexborHtmlParser() override;

    LexborHtmlParser(const LexborHtmlParser&) = delete;
    LexborHtmlParser& operator=(const LexborHtmlParser&) = delete;

    std::optional<core::models::Document> parseFile(
        const QString& absoluteFilePath, const QString& relativeDocPath) override;

private:
    // The CSS parser and compiled selector lists are created once in the
    // constructor and reused for every file - re-parsing a selector string
    // for each of the ~1500 ingest files would be wasted work.
    lxb_css_parser* m_cssParser = nullptr;
    lxb_selectors* m_selectors = nullptr;
    lxb_css_selector_list* m_articleSelectorList = nullptr;
    lxb_css_selector_list* m_shortdescSelectorList = nullptr;
};

} // namespace winccmcp::infrastructure::parsing

#endif // WINCCMCP_INFRASTRUCTURE_PARSING_LEXBORHTMLPARSER_H__1C2D3E4F_5AAB_4ACC_9455_ABC1D2E3F4A5__INCLUDED_
