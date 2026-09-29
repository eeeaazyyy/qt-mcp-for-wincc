#ifndef WINCCMCP_CORE_MODELS_RETRIEVEDCHUNK_H__3C4D5E6F_7081_4293_A415_C6D7E8F9A0B1__INCLUDED_
#define WINCCMCP_CORE_MODELS_RETRIEVEDCHUNK_H__3C4D5E6F_7081_4293_A415_C6D7E8F9A0B1__INCLUDED_

#include <QString>
#include <QVector>

namespace winccmcp::core::models {

// Result of a KNN search over the vector store: a chunk + the metadata of the
// document it belongs to + the distance to the query vector (smaller = closer).
struct RetrievedChunk {
    QString documentPath;
    QString documentTitle;
    int chunkIndex = 0;
    QString text;
    double distance = 0.0;
};

using RetrievedChunks = QVector<RetrievedChunk>;

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_RETRIEVEDCHUNK_H__3C4D5E6F_7081_4293_A415_C6D7E8F9A0B1__INCLUDED_
