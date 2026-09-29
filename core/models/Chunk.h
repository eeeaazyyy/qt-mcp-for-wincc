#ifndef WINCCMCP_CORE_MODELS_CHUNK_H__2B3C4D5E_6F70_4182_9304_B5C6D7E8F9A0__INCLUDED_
#define WINCCMCP_CORE_MODELS_CHUNK_H__2B3C4D5E_6F70_4182_9304_B5C6D7E8F9A0__INCLUDED_

#include <QString>
#include <QVector>

namespace winccmcp::core::models {

// One fragment of a document's text - the unit of embedding/search.
struct Chunk {
    qint64 id = 0;          // 0 until inserted into storage
    qint64 documentId = 0;
    int chunkIndex = 0;
    QString text;
};

using Embedding = QVector<float>;

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_CHUNK_H__2B3C4D5E_6F70_4182_9304_B5C6D7E8F9A0__INCLUDED_
