cmake_minimum_required(VERSION 3.24)
project(OrderBookMatchingEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(engine
  src/matching_engine.cpp
  src/order_book.cpp
)
target_include_directories(engine PUBLIC include)
target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic -Werror)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
  tests/unit/instrument_registration_test.cpp
  tests/unit/resting_limit_orders_test.cpp
  tests/unit/top_of_book_test.cpp
)
target_include_directories(unit_tests PRIVATE tests/support)
target_link_libraries(unit_tests PRIVATE engine GTest::gtest_main)
target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)

include(GoogleTest)
gtest_discover_tests(unit_tests)
