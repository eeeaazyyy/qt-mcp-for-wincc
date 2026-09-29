#ifndef WINCCMCP_CORE_INTERFACES_ITEXTCHUNKER_H__A9B0C1D2_E39F_4FB0_C7E2_D9EAFB0C1D2E__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_ITEXTCHUNKER_H__A9B0C1D2_E39F_4FB0_C7E2_D9EAFB0C1D2E__INCLUDED_

#include <QString>
#include <QVector>

namespace winccmcp::core::interfaces {

// Splits document text into fragments for embedding. Default implementation -
// ParagraphChunker (the same algorithm as chunker.py in the Python version).
class ITextChunker {
public:
    virtual ~ITextChunker() = default;

    virtual QVector<QString> split(const QString& text) const = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_ITEXTCHUNKER_H__A9B0C1D2_E39F_4FB0_C7E2_D9EAFB0C1D2E__INCLUDED_
