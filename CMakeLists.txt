cmake_minimum_required(VERSION 3.10.0)
project(futex-investigation VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(futex-investigation main.cpp)

