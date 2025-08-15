#include "gtest/gtest.h"
#include "../src/mylib/DataProcessor.h"

TEST(DataProcessorTest, InitialState) {
    DataProcessor dp;
    EXPECT_EQ(dp.getProcessedCount(), 0);
    EXPECT_EQ(dp.getDataSize(), 0);
}

TEST(DataProcessorTest, ProcessIncreasesCount) {
    DataProcessor dp;
    dp.processOnce();
    EXPECT_EQ(dp.getProcessedCount(), 1);
    EXPECT_EQ(dp.getDataSize(), 1);
}
