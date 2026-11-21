# C++ Low-Latency Trading Engine

A C++23 study project that starts with the lifecycle of a single order. The starter validates new orders, stores accepted orders by ID, and rejects duplicates. The command-line demo submits a buy order for 100 units of instrument 1 at 12,500 ticks. Later milestones introduce cancellation, replay, matching, and latency measurement.

## Build, test, and run

Run these commands from the repository root in Linux or WSL. The presets use CMake 3.28+, Ninja 1.11+, Clang 19, and its matching `clang-scan-deps`. On Ubuntu 24.04, the build tools are available as `cmake ninja-build clang-19 clang-tools-19 libclang-rt-19-dev`.

Set `VCPKG_ROOT` to your vcpkg checkout (for example, `export VCPKG_ROOT="$HOME/vcpkg"`). Both presets use its CMake toolchain to install GoogleTest from the root [vcpkg.json](vcpkg.json) during configuration.

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
./build/debug/trading_cli
```

The demo prints `ACCEPTED 101`. Each run starts a fresh processor.

The project’s [.clangd](.clangd) points the editor at `build/debug/compile_commands.json`. Configure and build Debug once so clangd has both the compiler flags and the compiled modules. Rebuilding Debug keeps them current. To inspect a different preset in the editor, configure and build that preset, then set `CompilationDatabase` to its build directory.

Run the tests with AddressSanitizer and UndefinedBehaviorSanitizer using the second preset.

```sh
cmake --preset asan-ubsan
cmake --build --preset asan-ubsan
ctest --preset asan-ubsan
```

## Ownership labs

- Run `./build/debug/copy_move_trace`. Predict how many destructors will run after constructing, copying, and moving the trace object.
- Run `./build/debug/owning_buffer`. Follow the allocation through a move, then extend the example to move-assign into a buffer that already owns storage.

## First exercise

Add `cancel(OrderId)` to the processor. Write a test that submits order 101, cancels it, then submits the same ID again. The final submission should return `DuplicateId`. Add cases for an unknown ID and a second cancellation. Once these transitions work, connect the processor to a text command fixture and compare its output to a golden file.

The [architecture](docs/architecture.md) describes the module boundaries. The [design decisions](docs/design-decisions.md) explain the representation choices and the [Week 1 retrospective](docs/week01-retrospective.md) has prompts for recording what you learned.
