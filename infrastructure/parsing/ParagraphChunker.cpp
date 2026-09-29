#include "infrastructure/parsing/ParagraphChunker.h"

#include <QStringList>

namespace winccmcp::infrastructure::parsing {

ParagraphChunker::ParagraphChunker(int chunkSize, int overlap) : m_chunkSize(chunkSize), m_overlap(overlap) {
}

QVector<QString> ParagraphChunker::split(const QString& text) const {
    QStringList paragraphs;
    for (const QString& line : text.split(QLatin1Char('\n'))) {
        const QString trimmed = line.trimmed();
        if (!trimmed.isEmpty()) {
            paragraphs << trimmed;
        }
    }
    if (paragraphs.isEmpty()) {
        return {};
    }

    QVector<QString> chunks;
    QString current;

    for (const QString& paragraph : paragraphs) {
        const QString candidate = current.isEmpty() ? paragraph : current + QLatin1Char('\n') + paragraph;

        if (candidate.size() <= m_chunkSize || current.isEmpty()) {
            current = candidate;
        } else {
            chunks.push_back(current);
            const QString tail = m_overlap > 0 ? current.right(m_overlap) : QString();
            current = tail.isEmpty() ? paragraph : tail + QLatin1Char('\n') + paragraph;
        }
    }

    if (!current.isEmpty()) {
        chunks.push_back(current);
    }

    return chunks;
}

} // namespace winccmcp::infrastructure::parsing
