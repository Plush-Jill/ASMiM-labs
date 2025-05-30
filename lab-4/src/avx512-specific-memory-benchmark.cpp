//
// Created by plush-jill on 5/31/25.
//

#ifndef AVX512_SPECIFIC_MEMORY_BENCHMARK_HPP
#define AVX512_SPECIFIC_MEMORY_BENCHMARK_HPP

#include <immintrin.h>
#include <chrono>
#include <iostream>
#include <cxxabi.h>
#include <vector>

class Avx512MemoryBenchmark {
    // static_assert(
        // std::is_arithmetic_v<T>,
        // "MemoryBenchmark can only be used with arithmetic types (int, float, double, etc.)"
        // );
private:
    static constexpr long long KB {1024};
    static constexpr long long MB {1024 * KB};
    static constexpr long long GB {1024 * MB};

    static constexpr int TEST_TIMES {3};

    static constexpr long long m_data_size = sizeof(__m512);
    static constexpr long long m_array_size = GB / m_data_size;
    static void check_number(__m512* number) {
        std::vector<char> vector (64, 0);
        _mm512_store_ps(vector.data(), *number);
        if (vector[1] == 1) {
            std::cout << "vector[1] == 1\n";
        }
    }

public:
    static constexpr long long get_array_size() {
        return m_array_size;
    }

    static void measure_read(const __m512* array) {
        __m512 sum {};
        auto read_iteration = [&](int i) {
                sum = _mm512_add_ps(
                    sum,
                    _mm512_load_ps(reinterpret_cast<const float*>(&array[i]))
                );
        };
        asm ("#MEASURE_READ_START");
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; /*i += 8*/ ++i) {
            // read_iteration(i);
            // read_iteration(i + 1);
            // read_iteration(i + 2);
            // read_iteration(i + 3);
            // read_iteration(i + 4);
            // read_iteration(i + 5);
            // read_iteration(i + 6);
            // read_iteration(i + 7);

                _mm512_add_ps(
                    sum,
                    _mm512_load_ps(reinterpret_cast<const float*>(&array[i]))
                );
        }
        const auto end = std::chrono::high_resolution_clock::now();

        check_number(&sum);
        report_performance("Read", start, end);
    }

    static void measure_write(__m512* array) {
        auto write_iteration = [&](int i) {
                _mm512_store_ps(reinterpret_cast<float*>(&array[i]), _mm512_set1_ps(1.0f));
        };
        asm ("#MEASURE_WRITE_START");
        const auto start = std::chrono::high_resolution_clock::now();

        for (int i {}; i < m_array_size; /*i += 8*/ ++i) {
            // write_iteration(i);
            // write_iteration(i + 1);
            // write_iteration(i + 2);
            // write_iteration(i + 3);
            // write_iteration(i + 4);
            // write_iteration(i + 5);
            // write_iteration(i + 6);
            // write_iteration(i + 7);

                _mm512_store_ps((&array[i]), _mm512_set1_ps(1.0f));
        }
        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&array[20]);

        report_performance("Write", start, end);
    }

    static void measure_replace(__m512* array1, __m512* array2) {
        auto replace_iteration = [&](int i) {
                _mm512_store_ps(reinterpret_cast<float*>(&array1[i]),
                    _mm512_load_ps(reinterpret_cast<const float*>(&array2[i])));
        };
        asm ("#MEASURE_REPLACE_START");
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; /*i += 8*/ ++i) {
            // replace_iteration(i);
            // replace_iteration(i + 1);
            // replace_iteration(i + 2);
            // replace_iteration(i + 3);
            // replace_iteration(i + 4);
            // replace_iteration(i + 5);
            // replace_iteration(i + 6);
            // replace_iteration(i + 7);

                _mm512_store_ps(reinterpret_cast<float*>(&array1[i]),
                    _mm512_load_ps(reinterpret_cast<const float*>(&array2[i])));
        }
        const auto end = std::chrono::high_resolution_clock::now();

        check_number(&array1[2]);
        check_number(&array2[2]);
        report_performance("Replace", start, end);
    }

    static void test_for_type() {

        {
            std::cout << demangle_type_name() << ":" << std::endl;
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<__m512> char_vector1 (get_array_size());
                    Avx512MemoryBenchmark::measure_read(char_vector1.data());

                }
            }
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<__m512> char_vector1 (get_array_size());
                    Avx512MemoryBenchmark::measure_write(char_vector1.data());

                }
            }
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<__m512> char_vector1 (get_array_size());
                    std::vector<__m512> char_vector2 (get_array_size());
                    Avx512MemoryBenchmark::measure_replace(char_vector1.data(), char_vector2.data());
                }
            }
            std::cout << "=========================\n" << std::endl;
        }
    }

private:
    static void report_performance(const char* operation,
                                 const std::chrono::time_point<std::chrono::high_resolution_clock>& start,
                                 const std::chrono::time_point<std::chrono::high_resolution_clock>& end) {
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        const double time_spent = static_cast<double>(duration.count()) * 1e-6;
        const double gb_per_second = static_cast<double>(m_array_size * m_data_size) / GB / time_spent;
        std::cout << operation << "<" << demangle_type_name() << ">" << ": " << gb_per_second << " GB/S" << std::endl;
    }

    static std::string demangle_type_name() {
        const char* mangled = typeid(__m512).name();
        int status;
        char* demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
        std::string result = status == 0 ? demangled : mangled;
        free(demangled);
        return result;
    }
};

int main() {
    Avx512MemoryBenchmark::test_for_type();
    return 0;
}


#endif //AVX512_SPECIFIC_MEMORY_BENCHMARK_HPP
