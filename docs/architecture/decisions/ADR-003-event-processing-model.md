# ADR-003: Deterministic event processing

- Status: Accepted
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

Two commands for the same order can produce different outcomes when reversed. A cancel after acceptance can succeed. A cancel before submission must report an unknown ID. Tests and replay need an explicit rule for which command takes effect first.

## 2. Decision

Process commands sequentially on one thread in arrival order. In Week 1, call order defines arrival order. For file replay, line order will define it. Complete each transition and produce its result before processing the next command. The processor owns the session's mutable order state. When matching is introduced, each instrument's book will likewise have one logical writer. Replaying the same ordered inputs from the same initial state must reproduce the same results in the same order.

## 3. Alternatives considered

A shared book with a lock could serialize updates, but lock acquisition alone would leave the relative order of concurrent submissions unspecified. A worker per instrument could process independent books in parallel, at the cost of defining routing and how results from multiple workers are ordered.

## 4. Consequences

The command sequence is sufficient to explain a state transition in a unit test. A slow command also delays every command behind it. Parallel processing across instruments remains a future option once a measured workload justifies it and replay tests can detect ordering changes.

## 5. Implementation implications

Today `submit` validates first, inserts a valid unused ID, and reports a duplicate on reuse. The next TRADING-004 test sequence is submit, cancel, resubmit with the same ID. Expect acceptance, successful cancellation, then duplicate rejection. A second cancellation and a cancellation of an unknown ID must each report their own rejection without changing state. TRADING-005 can then replay this sequence against a golden output file.

## 6. References

- [Submission implementation](../../../src/modules/trading.processor.cpp)
- [Current processor tests](../../../tests/test_order_processor.cpp)
- [ADR-006, order ID generation](ADR-006-order-id-generation.md)
