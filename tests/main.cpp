// A single shared main() for all tests (instead of GTest::gtest_main): some
// tests (SearchHistoryTableModelTest, QAbstractItemModelTester) need a live
// QCoreApplication, so it's created once for the whole test binary.
#include <gtest/gtest.h>

#include <QCoreApplication>

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
