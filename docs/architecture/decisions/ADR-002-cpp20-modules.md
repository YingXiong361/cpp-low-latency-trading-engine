# ADR-002: C++20 named modules

- Status: Accepted
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

The CLI and tests need the same definition of an order and the same processor API. The repository builds in C++23 mode. Named modules, introduced in C++20, let the trading core explicitly declare which types and operations its consumers can import.

## 2. Decision

Publish first-party trading-engine interfaces as named modules under `src/modules/`. Use `.cppm` for interface units and `.cpp` for separate implementation units. Executables and tests import the interfaces they use. Include conventional standard-library and third-party headers where needed. Defer `import std` until the selected toolchain can build it locally and in CI.

## 3. Alternatives considered

Header/source pairs offer broad tool support, but would leave the intended module boundaries untested. One large module would simplify the initial build graph while coupling consumers of basic domain types to the processor interface.

## 4. Consequences

A reviewer can see which declarations form a public interface and which imports it exposes. Builds depend on module dependency scanning and a compatible compiler, scanner, and generator. Each new interface unit needs an entry in the build target.

## 5. Implementation implications

`trading.order` re-exports `trading.types` and `trading.instrument`. In turn, `trading.processor` re-exports `trading.order` so a processor consumer can construct an order. CMake lists the four interface units in the `trading_core` target's `CXX_MODULES` file set. The processor implementation is a private source. A clean preset build tests whether CMake can order these compilations and link both the CLI and the tests (TRADING-001).

## 6. References

- [Trading core target and module file set](../../../CMakeLists.txt)
- [Order module interface](../../../src/modules/trading.order.cppm)
- [ADR-007, build and quality toolchain](ADR-007-build-and-quality-toolchain.md)
