#include "template-memory-benchmark.hpp"

int main() {
#ifndef SERVER
    // MemoryBenchmark<__m512>::test_for_type();
    // MemoryBenchmark<__m512>::test_for_type_threads(1);
    // MemoryBenchmark<__m512>::test_for_type_threads(2);
    // MemoryBenchmark<__m512>::test_for_type_threads(3);
    // MemoryBenchmark<__m512>::test_for_type_threads(4);
    // MemoryBenchmark<__m512>::test_for_type_threads(5);
    // MemoryBenchmark<__m512>::test_for_type_threads(6);
    // MemoryBenchmark<__m512>::measure_nt_copy();
    // MemoryBenchmark<__m512>::measure_nt_copy();
    // MemoryBenchmark<__m512>::measure_nt_copy();
    // MemoryBenchmark<__m512>::measure_nt_write();
    // MemoryBenchmark<__m512>::measure_nt_write();
    // MemoryBenchmark<__m512>::measure_nt_write();
    // MemoryBenchmark<__m512>::measure_nt_copy();
    // MemoryBenchmark<__m512>::measure_nt_copy();
    // MemoryBenchmark<__m512>::measure_nt_copy();
    // MemoryBenchmark<__m256>::test_for_type();
#endif

    MemoryBenchmark<__m128>::test_for_type_threads(6);
    // MemoryBenchmark<__m128>::test_for_type();
    // MemoryBenchmark<double>::test_for_type();
    // MemoryBenchmark<float>::test_for_type();
    // MemoryBenchmark<int>::test_for_type();
    // MemoryBenchmark<char>::test_for_type();
    // MemoryBenchmark<double>::test_lib_functions();
    return 0;
}
