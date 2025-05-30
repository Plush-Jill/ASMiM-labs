#include "template-memory-benchmark.hpp"

int main() {
#ifndef SERVER
    MemoryBenchmark<__m512>::test_for_type();
    MemoryBenchmark<__m256>::test_for_type();
#endif

    MemoryBenchmark<__m128>::test_for_type();
    MemoryBenchmark<double>::test_for_type();
    MemoryBenchmark<float>::test_for_type();
    MemoryBenchmark<int>::test_for_type();
    MemoryBenchmark<char>::test_for_type();
    return 0;
}
