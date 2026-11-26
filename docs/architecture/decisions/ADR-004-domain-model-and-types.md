# ADR-004: Trading domain model and types

- Status: Accepted (price representation superseded by [ADR-008](ADR-008-price-units-and-tick-bands.md) on 2026-11-24)
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

- An order for instrument 101 and an order numbered 101 carry two different identities.
- Prices must compare exactly at a tick boundary. For a given instrument, order entry must reject prices that fall between valid tick levels.
- Both the CLI and the processor need the same meaning for these values before lifecycle rules can be tested.

## 2. Decision

An order is a domain entity whose `OrderId` identifies it throughout its lifecycle. The processor borrows the submitted order by const reference and stores an accepted order by value. Its copy has the same logical identity and a lifetime independent of the caller's object.

- Require positive order and instrument identifiers. An order and its instrument may have the same numeric ID because their types distinguish their roles.
- Use distinct types for `OrderId`, `InstrumentId`, `PriceUnits`, and `Quantity`. A function expecting `(OrderId, Quantity)` must reject swapped arguments at compile time. Adding a quantity to an identifier has no domain meaning. e.g.

```c++
using OrderId = std::int64_t;
using Quantity = std::int64_t;

void no_type_safety(OrderId orderId, Quantity qty) {
    OrderId newId = orderId + qty;
    std::cout << newId << std::endl;
}

void test() {
    OrderId orderId{5};
    Quantity qty {4};
    no_type_safety(qty, orderId);
}
```

- Represent side with the scoped enumeration `Side`, whose named values are `Side::Buy` and `Side::Sell`.
- Quantity counts whole tradable units. A submitted order requires at least one unit. Submitting `Quantity{0}` returns `InvalidOrder` and leaves the ID available for a corrected submission. The instrument defines what one unit represents.
- A price counts fixed units in the instrument's quote currency. Keep the representation signed. For a spread quoted as leg A minus leg B, prices of 98 and 100 give a spread of −2; equal legs give zero. The instrument's permitted range and tick schedule determine which prices it accepts.
- Represent lifecycle status with a scoped enum when cancellation is introduced.
- Report routine rejections explicitly and preserve state on rejection.

## 3. Alternatives considered

**Primitive integer aliases** give fields readable names but allow unrelated IDs to pass through the same parameter.

Binary floating-point prices would require a **rounding policy** wherever prices are compared or used as book keys.

Shared ownership of a mutable order would allow the caller to change a previously accepted order.

## 4. Consequences

Integer units give exact equality for prices of the same instrument and scale.

Explicit construction makes each role visible at the call site.

Identifiers support equality within their own type. The processor hashes the numeric order ID for duplicate detection. Quantities and prices also support ordering within their own type.

Value members let `Order` follow the Rule of Zero and give the processor its own copy on acceptance.

## 5. Implementation implications

Each identifier takes a `std::int64_t` and rejects zero and negative values with `std::invalid_argument`. Callers check that unsigned or wider inputs fit the ID range of 1 through `INT64_MAX` before conversion. Construct an ID with `OrderId{5}` and read it with `id.value()` when printing or hashing.

`Quantity` takes a `std::int64_t` and rejects negative counts with `std::invalid_argument`. Callers check that unsigned or wider inputs fit before conversion. Addition above `INT64_MAX` and subtraction below zero raise `std::overflow_error`. Subtracting an exact fill leaves zero units remaining.

`PriceUnits` uses the full signed 64-bit range. Quantities and prices each compare within their own type, so `Quantity{4} < Quantity{5}` and `PriceUnits{-1} < PriceUnits{0}`. Comparing a quantity with a price is a compile-time error.

The current demo supports a positive-price instrument and still rejects zero and negative prices in `Order::is_valid()`. Before supporting spread orders, replace that demo check with validation against the instrument definition. Include negative, zero, and positive quotes in the spread tests, alongside a positive-only instrument that rejects the same nonpositive quotes.

## 6. References

- [Current domain types](../../../src/modules/trading.types.cppm)
- [Order value and validation](../../../src/modules/trading.order.cppm)
- [Domain tests](../../../tests/test_order.cpp)
- [ADR-005, instrument reference data](ADR-005-instrument-reference-data.md)
