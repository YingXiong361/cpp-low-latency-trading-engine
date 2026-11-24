# ADR-005: Instrument reference data

- Status: Proposed
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

A fixed price unit gives an integer price its meaning. A separate tick schedule determines which prices can be submitted for the instrument. An order carries an instrument ID, but the current `Instrument` value contains only that ID. A future gateway needs a consistent way to interpret prices, validate quantities, and check whether the instrument is open for trading.

## 2. Decision

Introduce an instrument reference-data component keyed by a stable, nonzero instrument ID. Each definition carries the symbol, instrument type, exchange, currency, fixed price unit, tick schedule, lot size, and trading status. A constant tick uses one band. At catalog load, check coverage of the supported price range, reject overlapping bands, and verify that bounds, grid origins, and increments are exactly representable in the fixed unit. See [ADR-008](ADR-008-price-units-and-tick-bands.md) for the boundary rule. A symbol may change while the instrument retains its identity. Start with a versioned local file or deterministic seed dataset. Select the same dataset version when replaying a recorded run.

## 3. Alternatives considered

Using a symbol as the primary key would make a rename change the identity recorded in orders. Copying metadata into every order would duplicate definitions and allow two orders for the same instrument to disagree. A database with a refresh adapter could support a larger catalog but would add an external dependency to the first replay fixture.

## 4. Consequences

The exchange and trading application can interpret the same order consistently. The catalog version becomes an input to replay alongside the commands. A database-backed adapter can later supply updated definitions through the same lookup interface. Effective dates and changes during a running session would need their own update policy.

## 5. Implementation implications

Week 1 validation currently checks that the instrument ID is nonzero. Catalog membership, tick and lot rules, and trading status are future gateway checks. A first catalog test can load one constant-tick and one banded instrument, look up each by ID, verify the boundary cases in ADR-008, and check that an unknown ID produces an explicit lookup failure. This fixture can run locally before any download or refresh mechanism exists.

## 6. References

- [Current instrument value](../../../src/modules/trading.instrument.cppm)
- [ADR-004, trading domain model and types](ADR-004-domain-model-and-types.md)
- [ADR-003, deterministic event processing](ADR-003-event-processing-model.md)
