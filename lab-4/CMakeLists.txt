cmake_minimum_required(VERSION 3.27)
project(lab_4)

set(CMAKE_CXX_STANDARD 20)

add_executable(template-measuring-server
        src/main.cpp
        src/template-memory-benchmark.cpp
        src/template-memory-benchmark.hpp
)

target_compile_options(template-measuring-server PRIVATE
        -msse2
#        -mavx2
#        -mavx512f
        -O0
)

target_compile_definitions(template-measuring-server PRIVATE SERVER)


add_executable(template-measuring
        src/main.cpp
        src/template-memory-benchmark.cpp
        src/template-memory-benchmark.hpp
)

target_compile_options(template-measuring PRIVATE
        -msse2
        -mavx2
        -mavx512f
        -O3
        -pthread
#        -funroll-loops
)



#add_executable(row-avx512-test src/row-avx512-test.cpp
#        src/row-avx512-test.cpp)


#add_executable(avx512-specific-memory-benchmark src/avx512-specific-memory-benchmark.cpp)

#target_compile_options(avx512-specific-memory-benchmark PRIVATE
#        -mavx512f
#        -O3
#        -fno-unroll-loops
        #        -funroll-loops
#)
