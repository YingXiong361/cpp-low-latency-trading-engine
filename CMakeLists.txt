cmake_minimum_required(VERSION 3.28)
project(trading_engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TRADING_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

add_library(trading_options INTERFACE)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(trading_options INTERFACE
        -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()
if(TRADING_SANITIZERS)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        message(FATAL_ERROR "The sanitizer preset requires Clang or GCC.")
    endif()
    target_compile_options(trading_options INTERFACE
        -fsanitize=address,undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer)
    target_link_options(trading_options INTERFACE -fsanitize=address,undefined)
endif()

add_library(trading_core STATIC)
target_sources(trading_core
    PUBLIC FILE_SET CXX_MODULES FILES
        src/modules/trading.types.cppm
        src/modules/trading.instrument.cppm
        src/modules/trading.order.cppm
        src/modules/trading.processor.cppm
    PRIVATE src/modules/trading.processor.cpp)
target_link_libraries(trading_core PRIVATE trading_options)

add_executable(trading_cli src/main.cpp)
target_link_libraries(trading_cli PRIVATE trading_core trading_options)

foreach(lab IN ITEMS copy_move_trace owning_buffer)
    add_executable(${lab} labs/${lab}.cpp)
    target_link_libraries(${lab} PRIVATE trading_options)
endforeach()

include(CTest)
if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)

    add_executable(trading_tests tests/test_order.cpp tests/test_order_processor.cpp)
    target_link_libraries(trading_tests PRIVATE trading_core trading_options GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(trading_tests)
    add_test(NAME labs.owning_buffer COMMAND owning_buffer)
endif()
