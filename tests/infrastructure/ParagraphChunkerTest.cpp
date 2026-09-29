#include <gtest/gtest.h>

#include "infrastructure/parsing/ParagraphChunker.h"

using winccmcp::infrastructure::parsing::ParagraphChunker;

namespace {

TEST(ParagraphChunkerTest, EmptyTextProducesNoChunks) {
    ParagraphChunker chunker(1200, 200);
    EXPECT_TRUE(chunker.split("").isEmpty());
    EXPECT_TRUE(chunker.split("\n\n\n").isEmpty());
}

TEST(ParagraphChunkerTest, ShortTextFitsInOneChunk) {
    ParagraphChunker chunker(1200, 200);
    const auto chunks = chunker.split("Title\nSome short description.\nAnother line.");
    ASSERT_EQ(chunks.size(), 1);
    EXPECT_EQ(chunks.first(), QStringLiteral("Title\nSome short description.\nAnother line."));
}

TEST(ParagraphChunkerTest, LongTextSplitsWithOverlap) {
    ParagraphChunker chunker(30, 10);

    QStringList paragraphs;
    for (int i = 0; i < 6; ++i) {
        paragraphs << QStringLiteral("Paragraph number %1 text").arg(i);
    }
    const auto chunks = chunker.split(paragraphs.join('\n'));

    ASSERT_GT(chunks.size(), 1);
    for (const auto& chunk : chunks) {
        EXPECT_LE(chunk.size(), 30 + 10 + 30); // a rough upper bound accounting for overlap + one paragraph
    }

    // The tail of the previous chunk should be at the start of the next one (overlap).
    const QString tail = chunks.first().right(10);
    EXPECT_TRUE(chunks.at(1).startsWith(tail));
}

TEST(ParagraphChunkerTest, BlankLinesAreIgnoredAsSeparators) {
    ParagraphChunker chunker(1200, 200);
    const auto chunks = chunker.split("First\n\n\nSecond\n   \nThird");
    ASSERT_EQ(chunks.size(), 1);
    EXPECT_EQ(chunks.first(), QStringLiteral("First\nSecond\nThird"));
}

} // namespace
