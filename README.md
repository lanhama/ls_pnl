# Trading Position System

A C++ system for managing trading positions with support for FIFO and LIFO accounting methods.

## Prerequisites

- C++17 compatible compiler (g++ or clang++)
- CMake (version 3.10 or higher)
- Google Test (gtest)


## Features

- Support for both FIFO and LIFO accounting methods
- Position tracking for multiple symbols
- PnL calculation for both long and short positions
- Position flipping (long to short and vice versa)
- Trade history tracking

## Building and running the project

1. Navigate to root of repo
2. Run `./bin/build.sh`
3. `./build/pnl_calculator <path-to-csv> <fifo|lifo>`

## Running tests
1. navigate to root of repo
2. `./bin/build_and_test.sh`
