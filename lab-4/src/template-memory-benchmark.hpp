//
// Created by plush-jill on 5/30/25.
//

#ifndef MEMORY_BENCHMARK_HPP
#define MEMORY_BENCHMARK_HPP

#include <immintrin.h>
#include <chrono>
#include <iostream>
#include <cxxabi.h>
#include <vector>


template<typename T>
class MemoryBenchmark {
    // static_assert(
        // std::is_arithmetic_v<T>,
        // "MemoryBenchmark can only be used with arithmetic types (int, float, double, etc.)"
        // );
private:
    static constexpr long long KB {1024};
    static constexpr long long MB {1024 * KB};
    static constexpr long long GB {1024 * MB};

    static constexpr int TEST_TIMES {3};

    static constexpr long long m_data_size = sizeof(T);
    static constexpr long long m_array_size = GB / m_data_size;
    static constexpr void check_number(T* number) {
        // if constexpr (std::is_same_v<T, __m256>) {
        //     if (const float* float_ptr = reinterpret_cast<float*>(number); float_ptr[0] == 50.0f) {
        //         std::cout << "number[0] == 50.0f\n";
        //     }
        // } else if constexpr (std::is_same_v<T, __m512>) {
        //     if (const auto* float_ptr = reinterpret_cast<float*>(number); float_ptr[0] == 50.0f) {
        //         std::cout << "number[0] == 50.0f\n";
        //     }
        // } else {
            if (*number == 50) {
                std::cout <<"number == 50\n";
            }
        // }
    }

public:
    static constexpr long long get_array_size() {
        return m_array_size;
    }

    static void measure_read(const T* array) {
        T sum {};
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; ++i) {
            // if constexpr (std::is_same_v<T, __m256>) {
            //     sum = _mm256_add_ps(
            //         sum,
            //         _mm256_load_ps(reinterpret_cast<const float*>(&array[i]))
            //     );
            // } else if constexpr (std::is_same_v<T, __m512>) {
            //     sum = _mm512_add_ps(
            //         sum,
            //         _mm512_load_ps(reinterpret_cast<const float*>(&array[i]))
            //     );
            // } else {
                sum += array[i];
            // }
        }
        const auto end = std::chrono::high_resolution_clock::now();

        check_number(&sum);
        report_performance("Read", start, end);
    }
    static void measure_write(T* array) {
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; ++i) {
            // if constexpr (std::is_same_v<T, __m256>) {
            //     _mm256_store_ps(reinterpret_cast<float*>(&array[i]), _mm256_set1_ps(1.0f));
            // } else if constexpr (std::is_same_v<T, __m512>) {
            //     _mm512_store_ps(reinterpret_cast<float*>(&array[i]), _mm512_set1_ps(1.0f));
            // } else {
                array[i] = 1;
            // }
        }
        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&array[20]);

        report_performance("Write", start, end);
    }
    static void measure_replace(T* array1, T* array2) {
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; ++i) {
            // if constexpr (std::is_same_v<T, __m256>) {
            //     _mm256_store_ps(reinterpret_cast<float*>(&array1[i]),
            //         _mm256_load_ps(reinterpret_cast<const float*>(&array2[i])));
            // } else if constexpr (std::is_same_v<T, __m512>) {
            //     _mm512_store_ps(reinterpret_cast<float*>(&array1[i]),
            //         _mm512_load_ps(reinterpret_cast<const float*>(&array2[i])));
            // } else {
                array1[i] = array2[i];
            // }
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
                    std::vector<T> char_vector1 (get_array_size());
                    MemoryBenchmark::measure_read(char_vector1.data());

                }
            }
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<T> char_vector1 (get_array_size());
                    MemoryBenchmark::measure_write(char_vector1.data());

                }
            }
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<T> char_vector1 (get_array_size());
                    std::vector<T> char_vector2 (get_array_size());
                    MemoryBenchmark::measure_replace(char_vector1.data(), char_vector2.data());
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
        const char* mangled = typeid(T).name();
        int status;
        char* demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
        std::string result = status == 0 ? demangled : mangled;
        free(demangled);
        return result;
    }
};

#endif //MEMORY_BENCHMARK_HPP
