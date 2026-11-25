# ADR-001: Platform architecture

- Status: Accepted
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

An exchange and its client have different views of the same order. The application can request a cancellation. Only the exchange can decide whether the order is still open. Sprint 1 needs a way to test that distinction before either side has a network connection.

## 2. Decision

Build the Exchange Simulator and Trading Application as logically separate parts of one platform. Host both in one process initially. The application sends commands and consumes execution reports and market data. The exchange owns validation, order lifecycle, and, in later milestones, per-instrument books and matching. Share the domain values that cross the boundary. Keep each side's mutable state private to its owner.

## 3. Alternatives considered

A CLI that edits the order map directly would be quick to demonstrate, but each new input adapter would have to reproduce the exchange rules. Separate executables would exercise a realistic transport while adding serialization, startup ordering, and connection failures to the first lifecycle tests.

## 4. Consequences

A unit test can issue the same command as the application without starting a server. Both sides share a failure domain while they run in one process. Passing commands and results as values gives a future transport adapter a well-defined interface to reproduce.

## 5. Implementation implications

Today `trading_cli` acts as a minimal client. It calls `OrderProcessor::submit` in `trading_core`, and the processor owns its accepted orders. TRADING-004 extends that interface with cancellation and tests the resulting state transitions. Once those results are specified, TRADING-005 can parse commands from a file and call the same interface. Execution reports and market data publication are later work.

## 6. References

- [CLI entry point](../../../src/main.cpp)
- [Processor interface](../../../src/modules/trading.processor.cppm)
- [ADR-003, deterministic event processing](ADR-003-event-processing-model.md)
