#include <gtest/gtest.h>

import trading.processor;

namespace {

using namespace trading;

TEST(OrderProcessorTest, RejectsASecondOrderWithTheSameId) {
    OrderProcessor processor;
    const Order buy{OrderId{101}, {InstrumentId{1}}, Side::Buy, PriceUnits{12500}, Quantity{100}};
    const Order sell{OrderId{101}, {InstrumentId{2}}, Side::Sell, PriceUnits{15000}, Quantity{50}};
    const auto accepted = processor.submit(buy);
    ASSERT_EQ(accepted.Result, SubmitResult::Accepted);
    EXPECT_EQ(accepted.RejectReason, SubmitRejectReason::None);
    const auto response = processor.submit(sell);
    ASSERT_EQ(response.Result, SubmitResult::Rejected);
    EXPECT_EQ(response.RejectReason, SubmitRejectReason::DuplicateId);
}

TEST(OrderProcessorTest, InvalidSubmissionLeavesTheIdAvailable) {
    OrderProcessor processor;
    Order order{OrderId{101}, {InstrumentId{1}}, Side::Buy, PriceUnits{12500}, Quantity{0}};
    const auto response = processor.submit(order);
    ASSERT_EQ(response.Result, SubmitResult::Rejected);
    EXPECT_EQ(response.RejectReason, SubmitRejectReason::InvalidOrder);
    order.quantity = Quantity{1};
    EXPECT_EQ(processor.submit(order).Result, SubmitResult::Accepted);
}

}
