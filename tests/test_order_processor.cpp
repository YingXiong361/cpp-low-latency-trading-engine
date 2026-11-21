#include <gtest/gtest.h>

import trading.processor;

namespace {

using namespace trading;

TEST(OrderProcessorTest, RejectsASecondOrderWithTheSameId) {
    OrderProcessor processor;
    const Order buy{101, {1}, Side::Buy, 12500, 100};
    const Order sell{101, {2}, Side::Sell, 15000, 50};
    EXPECT_EQ(processor.submit(buy), SubmitResult::Accepted);
    EXPECT_EQ(processor.submit(sell), SubmitResult::DuplicateId);
}

TEST(OrderProcessorTest, InvalidSubmissionLeavesTheIdAvailable) {
    OrderProcessor processor;
    Order order{101, {1}, Side::Buy, 12500, 0};
    EXPECT_EQ(processor.submit(order), SubmitResult::InvalidOrder);
    order.quantity = 1;
    EXPECT_EQ(processor.submit(order), SubmitResult::Accepted);
}

}
