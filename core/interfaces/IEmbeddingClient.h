#ifndef WINCCMCP_CORE_INTERFACES_IEMBEDDINGCLIENT_H__B4C5D6E7_F859_4A6B_D29D_E4F5A6B7C8D9__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_IEMBEDDINGCLIENT_H__B4C5D6E7_F859_4A6B_D29D_E4F5A6B7C8D9__INCLUDED_

#include <QString>
#include <QVector>

#include "core/models/Chunk.h"

namespace winccmcp::core::interfaces {

// Kept separate from ILlmChatClient (ISP): the embedding backend in LM Studio
// can be a different model, and tomorrow it could be a different service entirely.
class IEmbeddingClient {
public:
    virtual ~IEmbeddingClient() = default;

    // One request for the whole batch - saves round-trips to LM Studio.
    virtual QVector<models::Embedding> embed(const QVector<QString>& texts) = 0;

    // Vector dimension of the current embedding model (needed when creating
    // the vec0 table in SqliteVectorStore).
    virtual int embeddingDimensions() = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_IEMBEDDINGCLIENT_H__B4C5D6E7_F859_4A6B_D29D_E4F5A6B7C8D9__INCLUDED_
