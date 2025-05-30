cmake_minimum_required(VERSION 3.27)
project(lab_4)

set(CMAKE_CXX_STANDARD 17)

add_executable(template-measuring
        src/main.cpp
        src/template-memory-benchmark.cpp
        src/template-memory-benchmark.hpp
)

target_compile_options(template-measuring PRIVATE
        -mavx2
#        -mavx512f
        -O0
)

add_executable(row-avx512-test src/row-avx512-test.cpp
        src/row-avx512-test.cpp)