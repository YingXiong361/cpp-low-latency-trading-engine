# ADR-005: Instrument reference data

- Status: Proposed
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

A price of 100 ticks means 1.00 currency unit for an instrument with a 0.01 tick and 5.00 for one with a 0.05 tick. An order carries an instrument ID, but the current `Instrument` value contains only that ID. A future gateway needs a consistent way to interpret prices, validate quantities, and check whether the instrument is open for trading.

## 2. Decision

Introduce an instrument reference-data component keyed by a stable, nonzero instrument ID. Each definition carries the symbol, instrument type, exchange, currency, tick size, lot size, and trading status. A symbol may change while the instrument retains its identity. Start with a versioned local file or deterministic seed dataset. Select the same dataset version when replaying a recorded run.

## 3. Alternatives considered

Using a symbol as the primary key would make a rename change the identity recorded in orders. Copying metadata into every order would duplicate definitions and allow two orders for the same instrument to disagree. A database with a refresh adapter could support a larger catalog but would add an external dependency to the first replay fixture.

## 4. Consequences

The exchange and trading application can interpret the same order consistently. The catalog version becomes an input to replay alongside the commands. A database-backed adapter can later supply updated definitions through the same lookup interface. Effective dates and changes during a running session would need their own update policy.

## 5. Implementation implications

Week 1 validation currently checks that the instrument ID is nonzero. Catalog membership, tick and lot rules, and trading status are future gateway checks. A first catalog test can load two definitions with different tick sizes, look up each by ID, verify the two conversions above, and check that an unknown ID produces an explicit lookup failure. This fixture can run locally before any download or refresh mechanism exists.

## 6. References

- [Current instrument value](../../../src/modules/trading.instrument.cppm)
- [ADR-004, trading domain model and types](ADR-004-domain-model-and-types.md)
- [ADR-003, deterministic event processing](ADR-003-event-processing-model.md)
