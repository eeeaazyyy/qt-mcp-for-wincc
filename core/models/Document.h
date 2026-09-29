#ifndef WINCCMCP_CORE_MODELS_DOCUMENT_H__1A2B3C4D_5E6F_4071_8293_A4B5C6D7E8F9__INCLUDED_
#define WINCCMCP_CORE_MODELS_DOCUMENT_H__1A2B3C4D_5E6F_4071_8293_A4B5C6D7E8F9__INCLUDED_

#include <QString>

namespace winccmcp::core::models {

// One indexed HTML page of the WinCC OA documentation.
struct Document {
    qint64 id = 0;
    QString path;          // relative path from the documentation root, a stable key
    QString title;
    QString shortdesc;
    QString text;          // full cleaned article text (no navigation/scripts)
    QString contentHash;   // sha256(text) - for incremental ingest
};

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_DOCUMENT_H__1A2B3C4D_5E6F_4071_8293_A4B5C6D7E8F9__INCLUDED_
