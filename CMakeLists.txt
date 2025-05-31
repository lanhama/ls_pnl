cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(pnl_calculator VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find GTest package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Copy test resources to build directory
configure_file(${CMAKE_SOURCE_DIR}/tests/test_trades.csv ${CMAKE_BINARY_DIR}/test_trades.csv COPYONLY)

# Add main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Add test executable
add_executable(pnl_calculator_tests
    tests/main.cpp
    tests/csv_parser_test.cpp
)

# Link test executable with GTest
target_link_libraries(pnl_calculator_tests ${GTEST_LIBRARIES} pthread)

# Enable testing
enable_testing()
add_test(NAME pnl_calculator_tests COMMAND pnl_calculator_tests) 