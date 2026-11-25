# Sprint 1 architecture decisions

The target platform has an exchange simulator and a trading application. The first runnable piece is smaller. A CLI submits one order to an in-process `OrderProcessor` and prints `ACCEPTED 101`. The tests exercise the same processor with invalid orders and duplicate IDs. [Build and run the demo](../../README.md#build-test-and-run).

## Decision records

| Ticket | Design question | Status |
| --- | --- | --- |
| ARCH-001 | [Where is the boundary between the exchange and its client?](decisions/ADR-001-platform-architecture.md) | Accepted |
| ARCH-002 | [How does a component publish its C++ interface?](decisions/ADR-002-cpp20-modules.md) | Accepted |
| ARCH-003 | [Who owns mutable state, and in what order do commands take effect?](decisions/ADR-003-event-processing-model.md) | Accepted |
| ARCH-004 | [How do orders represent identity, price, quantity, and side?](decisions/ADR-004-domain-model-and-types.md) | Accepted (price representation superseded by ARCH-008) |
| ARCH-005 | [Where does an instrument get its tick size and lot size?](decisions/ADR-005-instrument-reference-data.md) | Proposed |
| ARCH-006 | [Who assigns an order ID, and how long is it unique?](decisions/ADR-006-order-id-generation.md) | Proposed |
| ARCH-007 | [How can a contributor reproduce the build and its checks?](decisions/ADR-007-build-and-quality-toolchain.md) | Proposed |
| ARCH-008 | [How are prices encoded and validated across tick bands?](decisions/ADR-008-price-units-and-tick-bands.md) | Accepted |

An accepted record guides implementation. Each record identifies the code available today and the next check that would demonstrate the decision in practice. A proposed record is open for review, even where a working example already exists.

## From the demo to replay

The next Sprint 1 exercise is cancellation (TRADING-004). Submit order 101, cancel it, then submit 101 again. The last command should return `DuplicateId`. Also test cancellation of an unknown ID and a second cancellation of 101. Those processor tests establish the results that the file-driven CLI (TRADING-005) should reproduce from a fixture. Order books, matching, and strategy execution belong to later milestones.

When a decision changes, add a superseding ADR that explains the new evidence and any migration. Mark the original record as superseded and link the two so readers can follow the decision history.
