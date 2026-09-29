#ifndef WINCCMCP_CORE_MODELS_VECTORMATCH_H__8192A3B4_C526_4748_A96A_B1C2D3E4F5A6__INCLUDED_
#define WINCCMCP_CORE_MODELS_VECTORMATCH_H__8192A3B4_C526_4748_A96A_B1C2D3E4F5A6__INCLUDED_

#include <QVector>

namespace winccmcp::core::models {

// Raw result of a KNN search in IVectorStore, before attaching the document's
// text/metadata - that's the job of IDocumentRepository::resolveChunks().
struct VectorMatch {
    qint64 chunkId = 0;
    double distance = 0.0;
};

using VectorMatches = QVector<VectorMatch>;

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_VECTORMATCH_H__8192A3B4_C526_4748_A96A_B1C2D3E4F5A6__INCLUDED_
