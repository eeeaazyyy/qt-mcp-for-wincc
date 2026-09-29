#include <gtest/gtest.h>

#include <QAbstractItemModelTester>

#include "app/history/SearchHistoryTableModel.h"

using winccmcp::app::history::SearchHistoryTableModel;
using winccmcp::core::models::ChatHistoryRecord;

namespace {

TEST(SearchHistoryTableModelTest, SatisfiesQAbstractItemModelContract) {
    SearchHistoryTableModel model;

    ChatHistoryRecord r1;
    r1.query = "dpGet vs dpConnect";
    r1.answer = "dpGet reads once, dpConnect subscribes.";
    r1.timestamp = QDateTime::currentDateTime();
    r1.durationMs = 850;
    model.setRecords({r1});

    // Fails via qFatal if the QAbstractItemModel contract is violated - the
    // mere fact that the test reaches the end is itself the check.
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::Fatal);

    ChatHistoryRecord r2;
    r2.query = "abs()";
    r2.answer = "Returns the absolute value.";
    r2.durationMs = 120;
    model.addRecord(r2);

    EXPECT_EQ(model.rowCount(), 2);
    EXPECT_EQ(model.recordAt(1).query, QStringLiteral("abs()"));
}

} // namespace
