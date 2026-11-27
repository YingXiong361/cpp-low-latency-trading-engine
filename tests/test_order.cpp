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
static_assert(std::is_aggregate_v<Instrument>);
static_assert(!std::is_default_constructible_v<Instrument>);
static_assert(std::is_invocable_v<SubmitArgs, OrderId, Quantity>);
static_assert(!std::is_invocable_v<SubmitArgs, Quantity, OrderId>);
static_assert(!Addable<OrderId, Quantity>);
static_assert(!std::is_assignable_v<OrderId&, InstrumentId>);
static_assert(!std::is_assignable_v<PriceUnits&, Quantity>);
static_assert(!std::is_convertible_v<std::int64_t, OrderId>);
static_assert(!std::is_convertible_v<std::int64_t, InstrumentId>);
static_assert(std::totally_ordered<Quantity> && std::totally_ordered<PriceUnits>);
static_assert(!std::totally_ordered_with<Quantity, PriceUnits>);

TEST(IdentifierTest, RejectsNonpositiveValuesAtConstruction) {
    EXPECT_THROW((void)OrderId{-1}, std::invalid_argument);
    EXPECT_THROW((void)InstrumentId{-1}, std::invalid_argument);
    EXPECT_THROW((void)OrderId{0}, std::invalid_argument);
    EXPECT_THROW((void)InstrumentId{0}, std::invalid_argument);
}

TEST(IdentifierTest, AcceptsMaximumSignedValue) {
    constexpr auto max = std::numeric_limits<std::int64_t>::max();
    static_assert(OrderId{max}.value() == max);
    static_assert(InstrumentId{max}.value() == max);
}

TEST(DomainTypesTest, QuantityAcceptsNonnegativeCounts) {
    constexpr auto max = std::numeric_limits<std::int64_t>::max();
    static_assert(Quantity{}.value() == 0);
    static_assert(Quantity{max}.value() == max);
    EXPECT_THROW((void)Quantity{-1}, std::invalid_argument);
}

TEST(DomainTypesTest, ComparesQuantitiesAndSignedPriceUnits) {
    EXPECT_EQ(Quantity{0}, Quantity{0});
    EXPECT_LT(Quantity{0}, Quantity{1});
    EXPECT_GT(Quantity{std::numeric_limits<std::int64_t>::max()}, Quantity{1});
    EXPECT_EQ(PriceUnits{-1}, PriceUnits{-1});
    EXPECT_LT(PriceUnits{std::numeric_limits<std::int64_t>::min()}, PriceUnits{0});
    EXPECT_LT(PriceUnits{0}, PriceUnits{1});
}

TEST(DomainTypesTest, QuantityArithmeticChecksBothBounds) {
    constexpr Quantity max{std::numeric_limits<std::int64_t>::max()};
    static_assert(max + Quantity{0} == max);
    static_assert(max - Quantity{1} + Quantity{1} == max);
    static_assert(Quantity{3} - Quantity{3} == Quantity{0});
    EXPECT_THROW(max + Quantity{1}, std::overflow_error);
    EXPECT_THROW(Quantity{3} - Quantity{5}, std::overflow_error);
}

TEST(DomainTypesTest, PriceArithmeticChecksBothBounds) {
    constexpr PriceUnits min{std::numeric_limits<std::int64_t>::min()};
    constexpr PriceUnits max{std::numeric_limits<std::int64_t>::max()};
    static_assert(min + max == PriceUnits{-1});
    static_assert(PriceUnits{-1} - min == max);
    EXPECT_THROW(max + PriceUnits{1}, std::overflow_error);
    EXPECT_THROW(min + PriceUnits{-1}, std::overflow_error);
    EXPECT_THROW(min - PriceUnits{1}, std::overflow_error);
    EXPECT_THROW(max - PriceUnits{-1}, std::overflow_error);
}

TEST(DomainTypesTest, UpdatingACopiedPriceLeavesTheOriginalIntact) {
    static_assert(PriceUnits{}.value() == 0);
    static_assert(std::same_as<decltype(PriceUnits{}.value()), std::int64_t>);
    const PriceUnits original{-5};
    auto updated = original;
    updated = updated + PriceUnits{8};
    EXPECT_EQ(updated.value(), 3);
    EXPECT_EQ(original.value(), -5);
}

TEST(OrderTest, AcceptsMaximumRepresentableFields) {
    const Order order{
        OrderId{std::numeric_limits<std::int64_t>::max()},
        {InstrumentId{std::numeric_limits<std::int64_t>::max()}},
        Side::Buy,
        PriceUnits{std::numeric_limits<std::int64_t>::max()},
        Quantity{std::numeric_limits<std::int64_t>::max()}
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
