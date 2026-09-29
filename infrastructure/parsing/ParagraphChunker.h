#ifndef WINCCMCP_INFRASTRUCTURE_PARSING_PARAGRAPHCHUNKER_H__2D3E4F5A_6BBC_4BDD_A566_BCD1E2F3A4B5__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PARSING_PARAGRAPHCHUNKER_H__2D3E4F5A_6BBC_4BDD_A566_BCD1E2F3A4B5__INCLUDED_

#include "core/interfaces/ITextChunker.h"

namespace winccmcp::infrastructure::parsing {

// The same algorithm as chunker.py in the Python version: split text on
// paragraphs (a blank line is a boundary), glue neighboring paragraphs until
// chunkSize characters is reached, and keep overlap characters of "tail" at
// the start of the next chunk so context isn't lost at the boundary.
class ParagraphChunker : public core::interfaces::ITextChunker {
public:
    explicit ParagraphChunker(int chunkSize = 1200, int overlap = 200);

    QVector<QString> split(const QString& text) const override;

private:
    int m_chunkSize;
    int m_overlap;
};

} // namespace winccmcp::infrastructure::parsing

#endif // WINCCMCP_INFRASTRUCTURE_PARSING_PARAGRAPHCHUNKER_H__2D3E4F5A_6BBC_4BDD_A566_BCD1E2F3A4B5__INCLUDED_
