cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(pnl_calculator VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp) 