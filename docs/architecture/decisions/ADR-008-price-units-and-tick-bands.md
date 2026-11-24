# ADR-008: Fixed price units and tick bands

- Status: Accepted
- Date: 2026-11-24
- Decision owners: Project Engineering Team
- Supersedes the price representation in [ADR-004](ADR-004-domain-model-and-types.md)

## Context

An instrument can trade in increments of 0.01 below 10.00 and 0.10 from 10.00 upward. Counting valid ticks from zero is reversible, but decoding that count requires the tick schedule. A quoted price should retain its meaning when it crosses a band boundary.

## Decision

Store each positive order price as a signed 64-bit integer count of fixed units (`PriceUnits`). Define the unit in the instrument's quote currency and keep it constant across all bands. Choose a unit that represents every supported tick increment and band boundary exactly, with enough integer range for the maximum price. For example, the smaller of two increments, 0.02 and 0.05, cannot represent both. A unit of 0.01 represents them as 2 and 5.

Use inclusive lower and exclusive upper band bounds. Express bounds and positive tick increments in integer units. For a price within its band, validate `(price_units - grid_origin_units) % tick_units == 0`. In the example below the grid starts at each band's lower bound.

| Quote | Units at 0.01 | Applicable tick | Result |
| --- | ---: | ---: | --- |
| 9.99 | 999 | 1 unit | Valid |
| 10.00 | 1000 | 10 units | Valid at the upper band's lower bound |
| 10.01 | 1001 | 10 units | Off-tick |
| 10.10 | 1010 | 10 units | Valid |

## Consequences

At order entry, parse the decimal price exactly once, check the supported range, and reject unrepresentable or off-tick values. Store and compare validated integers throughout the book for that instrument. Format the quoted price when emitting decimal output. Operations such as finding the next valid quote still consult the applicable tick schedule.

## Implementation and migration

The order already stores its price as `PriceUnits`, a signed 64-bit integer. The instrument currently carries only its ID. Converting units to a quoted price and validating tick bands require the instrument definitions proposed in [ADR-005](ADR-005-instrument-reference-data.md). Convert existing tick-count fixtures using their original instrument definition. For example, 100 legacy ticks of 0.05 become 500 units of 0.01. A fixture whose original tick size is unknown needs that information before it can be converted.

Test both sides of a band boundary, exact decimal round trips, unrepresentable prices, overflow, and replay against the recorded reference-data version.
