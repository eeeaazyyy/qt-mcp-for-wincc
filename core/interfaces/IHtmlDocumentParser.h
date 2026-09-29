#ifndef WINCCMCP_CORE_INTERFACES_IHTMLDOCUMENTPARSER_H__F8A9B0C1_D28E_4EAF_B6D1_C8D9EAFB0C1D__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_IHTMLDOCUMENTPARSER_H__F8A9B0C1_D28E_4EAF_B6D1_C8D9EAFB0C1D__INCLUDED_

#include <optional>

#include <QString>

#include "core/models/Document.h"

namespace winccmcp::core::interfaces {

// Default implementation - LexborHtmlParser: strips WebHelp/DITA chrome
// (navigation) and keeps only the article text (analogous to
// BeautifulSoup.select_one in the Python version of this same project).
class IHtmlDocumentParser {
public:
    virtual ~IHtmlDocumentParser() = default;

    // Returns nullopt for utility pages with no useful content
    // (index.html, search.html, etc.).
    virtual std::optional<models::Document> parseFile(
        const QString& absoluteFilePath, const QString& relativeDocPath) = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_IHTMLDOCUMENTPARSER_H__F8A9B0C1_D28E_4EAF_B6D1_C8D9EAFB0C1D__INCLUDED_
