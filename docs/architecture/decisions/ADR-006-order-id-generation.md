# ADR-006: Order ID generation

- Status: Proposed
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

The Week 1 CLI supplies order ID 101, and the processor checks whether it has accepted that ID in the current session. A future gateway will need to correlate a client's request with an exchange-assigned order. A process restart also needs to be visible when reading logs that span multiple runs.

## 2. Decision

Keep the client order ID as the client's correlation key. Give the exchange its own process-local, monotonically increasing sequence for exchange order IDs, starting at 1 in each engine run. Reserve zero. Never reuse an issued ID within a run, including after cancellation. Include the engine-run identifier alongside the exchange order ID in logs and events so that order 1 from two runs can be distinguished.

## 3. Alternatives considered

Reusing the client ID as the exchange ID would require independent clients to coordinate their choices. UUIDs would avoid a central counter but would make recorded fixtures harder to read and reproduce. A durable sequence or an external sequencer could preserve uniqueness across restarts at the cost of persistence, recovery, and coordination.

## 4. Consequences

Tests can predict the next exchange ID given the same initial sequence and inputs. An exchange ID alone is meaningful only within its run. Consumers that retain records across restarts must preserve the run ID too. Sequence exhaustion must produce an explicit failure before wraparound could issue zero or reuse an ID.

## 5. Implementation implications

Keep the Week 1 processor API and its caller-supplied fixture IDs while testing duplicate rejection and cancellation (TRADING-004). When the gateway is introduced, give client and exchange IDs distinct types and return their association in an acknowledgement. Test with two simulated runs. The first issues IDs 1 and 2, the second issues ID 1, and the recorded run/ID pairs distinguish all three orders. Durable allocation can be revisited when restart recovery becomes a requirement.

## 6. References

- [Current CLI order construction](../../../src/main.cpp)
- [Duplicate-ID tests](../../../tests/test_order_processor.cpp)
- [ADR-004, trading domain model and types](ADR-004-domain-model-and-types.md)
