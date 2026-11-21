#include <gtest/gtest.h>

import trading.order;

namespace {

using namespace trading;

TEST(OrderTest, AcceptsPositiveIntegerTicksAndQuantity) {
    const Order order{1, {1}, Side::Sell, 1, 1};
    EXPECT_TRUE(order.is_valid());
}

TEST(OrderTest, RequiresIdentifiersPriceQuantityAndKnownSide) {
    for (const Order order : {
        Order{0, {1}, Side::Buy, 1, 1},
        Order{1, {0}, Side::Buy, 1, 1},
        Order{1, {1}, Side::Buy, 0, 1},
        Order{1, {1}, Side::Buy, -1, 1},
        Order{1, {1}, Side::Buy, 1, 0},
        Order{1, {1}, static_cast<Side>(99), 1, 1}
    }) {
        EXPECT_FALSE(order.is_valid());
    }
}

}
