#include <gtest/gtest.h>
#include <concepts>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

import trading.order;

namespace {

using namespace trading;

template<class A, class B>
concept Addable = requires(A a, B b) { a + b; };

using SubmitArgs = void(OrderId, Quantity);
static_assert(std::is_invocable_v<SubmitArgs, OrderId, Quantity>);
static_assert(!std::is_invocable_v<SubmitArgs, Quantity, OrderId>);
static_assert(!Addable<OrderId, Quantity>);
static_assert(!std::is_assignable_v<OrderId&, InstrumentId>);
static_assert(!std::is_assignable_v<PriceUnits&, Quantity>);
static_assert(!std::is_convertible_v<std::uint64_t, OrderId>);
static_assert(!std::is_convertible_v<std::uint64_t, InstrumentId>);
static_assert(std::totally_ordered<Quantity> && std::totally_ordered<PriceUnits>);
static_assert(!std::totally_ordered_with<Quantity, PriceUnits>);

TEST(IdentifierTest, RejectsZeroAtConstruction) {
    EXPECT_THROW((void)OrderId{0}, std::invalid_argument);
    EXPECT_THROW((void)InstrumentId{0}, std::invalid_argument);
}

TEST(DomainTypesTest, ComparesQuantitiesAndSignedPriceUnits) {
    EXPECT_EQ(Quantity{0}, Quantity{0});
    EXPECT_LT(Quantity{0}, Quantity{1});
    EXPECT_GT(Quantity{std::numeric_limits<std::uint64_t>::max()}, Quantity{1});
    EXPECT_EQ(PriceUnits{-1}, PriceUnits{-1});
    EXPECT_LT(PriceUnits{std::numeric_limits<std::int64_t>::min()}, PriceUnits{0});
    EXPECT_LT(PriceUnits{0}, PriceUnits{1});
}

TEST(OrderTest, AcceptsMaximumRepresentableFields) {
    const Order order{
        OrderId{std::numeric_limits<std::uint64_t>::max()},
        {InstrumentId{std::numeric_limits<std::uint64_t>::max()}},
        Side::Buy,
        PriceUnits{std::numeric_limits<std::int64_t>::max()},
        Quantity{std::numeric_limits<std::uint64_t>::max()}
    };
    EXPECT_TRUE(order.is_valid());
}

TEST(OrderTest, AcceptsPositivePriceUnitsAndQuantity) {
    const Order order{OrderId{1}, {InstrumentId{1}}, Side::Sell, PriceUnits{1}, Quantity{1}};
    EXPECT_TRUE(order.is_valid());
}

TEST(OrderTest, RequiresPositivePricePositiveQuantityAndKnownSide) {
    for (const Order order : {
        Order{OrderId{1}, {InstrumentId{1}}, Side::Buy, PriceUnits{0}, Quantity{1}},
        Order{OrderId{1}, {InstrumentId{1}}, Side::Buy, PriceUnits{-1}, Quantity{1}},
        Order{OrderId{1}, {InstrumentId{1}}, Side::Buy, PriceUnits{1}, Quantity{0}},
        Order{OrderId{1}, {InstrumentId{1}}, static_cast<Side>(99), PriceUnits{1}, Quantity{1}}
    }) {
        EXPECT_FALSE(order.is_valid());
    }
}

}
