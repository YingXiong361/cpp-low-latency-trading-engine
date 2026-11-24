# ADR-004: Trading domain model and types

- Status: Accepted (price representation superseded by [ADR-008](ADR-008-price-units-and-tick-bands.md) on 2026-11-24)
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

An order for instrument 101 and an order numbered 101 carry two different identities. Prices must compare exactly at a tick boundary. Both the CLI and the processor need the same meaning for these values before lifecycle rules can be tested.

## 2. Decision

Model an order as a value with distinct, nonzero order and instrument identifiers, a scoped buy/sell side, a positive price in integer ticks, and a positive whole-number quantity. Use strong identifier types so interchanging them is a compile-time error. Interpret tick counts using the instrument's tick size. Represent lifecycle status with a scoped enum when cancellation is introduced. Report routine rejections explicitly and preserve state on rejection.

## 3. Alternatives considered

Primitive integer aliases give fields readable names but allow unrelated IDs to pass through the same parameter. Binary floating-point prices would require a rounding policy wherever prices are compared or used as book keys. Shared ownership of a mutable order would allow the caller to change a previously accepted order.

## 4. Consequences

Integer ticks give exact equality for prices of the same instrument. Converting a displayed price into ticks requires the instrument definition and a policy for off-tick input. Identifier wrappers need explicit construction, equality, and support for lookup by key. Value members let `Order` follow the Rule of Zero and give the processor its own copy on acceptance.

## 5. Implementation implications

Today `OrderId` and `Quantity` are 64-bit unsigned aliases, `PriceTicks` is a signed 64-bit alias, and `Instrument::id` is a 32-bit unsigned field. `Side` is a scoped enum. The tests exercise zero IDs, nonpositive prices, zero quantity, and an invalid side. Completing TRADING-002 includes introducing distinct identifier wrappers and checking boundary values and compile-time ID separation. These types then give the processor a tested vocabulary for TRADING-004.

## 6. References

- [Current domain types](../../../src/modules/trading.types.cppm)
- [Order value and validation](../../../src/modules/trading.order.cppm)
- [Domain tests](../../../tests/test_order.cpp)
- [ADR-005, instrument reference data](ADR-005-instrument-reference-data.md)
