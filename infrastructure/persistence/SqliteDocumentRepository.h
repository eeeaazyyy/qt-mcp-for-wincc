#ifndef WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEDOCUMENTREPOSITORY_H__FA0B1C2D_3E9A_4AAB_7233_8E9FA0B1C2D9__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEDOCUMENTREPOSITORY_H__FA0B1C2D_3E9A_4AAB_7233_8E9FA0B1C2D9__INCLUDED_

#include "core/interfaces/IDocumentRepository.h"
#include "infrastructure/persistence/SqliteConnection.h"

namespace winccmcp::infrastructure::persistence {

class SqliteDocumentRepository : public core::interfaces::IDocumentRepository {
public:
    explicit SqliteDocumentRepository(SqliteConnection& connection);

    std::optional<core::models::Document> findByPath(const QString& path) override;
    QVector<core::models::Document> search(const QString& substring, int limit) override;
    std::pair<qint64, bool> upsertDocument(const core::models::Document& doc) override;
    QVector<qint64> chunkIdsForDocument(qint64 documentId) override;
    QVector<core::models::Chunk> replaceChunks(qint64 documentId, const QVector<QString>& chunkTexts) override;
    core::models::RetrievedChunks resolveChunks(const core::models::VectorMatches& matches) override;

private:
    SqliteConnection& m_connection;
};

} // namespace winccmcp::infrastructure::persistence

#endif // WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEDOCUMENTREPOSITORY_H__FA0B1C2D_3E9A_4AAB_7233_8E9FA0B1C2D9__INCLUDED_
