#include <gtest/gtest.h>

import trading.processor;

namespace {

using namespace trading;

TEST(OrderProcessorTest, RejectsASecondOrderWithTheSameId) {
    OrderProcessor processor;
    const Order buy{101, {1}, Side::Buy, 12500, 100};
    const Order sell{101, {2}, Side::Sell, 15000, 50};
    const auto accepted = processor.submit(buy);
    ASSERT_EQ(accepted.Result, SubmitResult::Accepted);
    EXPECT_EQ(accepted.RejectReason, SubmitRejectReason::None);
    const auto response = processor.submit(sell);
    ASSERT_EQ(response.Result, SubmitResult::Rejected);
    EXPECT_EQ(response.RejectReason, SubmitRejectReason::DuplicateId);
}

TEST(OrderProcessorTest, InvalidSubmissionLeavesTheIdAvailable) {
    OrderProcessor processor;
    Order order{101, {1}, Side::Buy, 12500, 0};
    const auto response = processor.submit(order);
    ASSERT_EQ(response.Result, SubmitResult::Rejected);
    EXPECT_EQ(response.RejectReason, SubmitRejectReason::InvalidOrder);
    order.quantity = 1;
    EXPECT_EQ(processor.submit(order).Result, SubmitResult::Accepted);
}

}
