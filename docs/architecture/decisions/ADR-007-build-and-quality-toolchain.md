# ADR-007: Build and quality toolchain

- Status: Proposed
- Date: 2026-11-21
- Decision owners: Project Engineering Team

## 1. Context

An editor can resolve an import even when a clean command-line build cannot compile the modules in dependency order. Before extending the processor, a contributor needs to build the CLI and tests from the same sources and reproduce a failure on Linux. The repository already provides presets and CI for the proposed baseline.

## 2. Decision

Use C++23 mode with CMake 3.28 or newer, Ninja, Clang 19 and matching `clang-scan-deps`. Resolve GoogleTest from the vcpkg manifest and its pinned registry baseline. Run GoogleTest through CTest. Use the `debug` preset for normal development and `asan-ubsan` for AddressSanitizer and UndefinedBehaviorSanitizer. Apply the project's warning flags to first-party targets and run both presets in Linux CI.

## 3. Alternatives considered

Per-developer compiler commands would make module scanning and warning flags hard to reproduce. A system-installed GoogleTest would leave its version dependent on the host. Supporting multiple compiler families immediately would expand the compatibility checks before the core lifecycle is complete. Another compiler can be evaluated with the same clean build and test criteria.

## 4. Consequences

Local WSL development and Ubuntu CI share configure, build, and test entry points. Contributors need a compatible module scanner, sanitizer runtimes, and a vcpkg checkout. Sanitizer instrumentation adds runtime overhead. Latency comparisons will require a separate measured build and a defined workload in a later milestone.

## 5. Implementation implications

TRADING-001 establishes the proof of concept. From a clean checkout with `VCPKG_ROOT` set, run the following from the repository root. The CLI should print `ACCEPTED 101`.

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
./build/debug/trading_cli

cmake --preset asan-ubsan
cmake --build --preset asan-ubsan
ctest --preset asan-ubsan
```

CMake currently enables `-Wall -Wextra -Wpedantic -Wconversion -Wshadow` for Clang and GCC. The CI matrix builds and tests both presets. This gives the domain tests (TRADING-002) and ownership labs (TRADING-003) a common way to expose regressions.

## 6. References

- [CMake targets and diagnostic flags](../../../CMakeLists.txt)
- [Configure, build, and test presets](../../../CMakePresets.json)
- [vcpkg manifest and baseline](../../../vcpkg.json)
- [Linux CI workflow](../../../.github/workflows/ci.yml)
