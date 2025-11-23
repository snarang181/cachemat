cmake_minimum_required(VERSION 3.16)
project(cachemat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    cachemat
    src/matrix.cpp
    src/matmul_naive.cpp
    src/matmul_blocked.cpp
    src/autotune.cpp
)

target_include_directories(
    cachemat
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    bench_matmul
    benchmarks/bench_matmul.cpp
)

target_link_libraries(
    bench_matmul
    PRIVATE cachemat
)