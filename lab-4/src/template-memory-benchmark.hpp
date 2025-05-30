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
#include <memory.h>
#include <atomic>
#include <thread>
#include <barrier>
#include <numeric>


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
    static constexpr int UNROLL_SIZE {1};

    static constexpr long long m_data_size = sizeof(T);
    static constexpr long long m_array_size = GB / m_data_size;
    static constexpr void check_number(T* number) {
        if constexpr (std::is_same_v<T, __m128>) {
            if (const float* float_ptr = reinterpret_cast<float*>(number); float_ptr[0] == 50.0f) {
                std::cout << "number[0] == 50.0f\n";
            }
#ifndef SERVER
        } else if constexpr (std::is_same_v<T, __m256>) {
            if (const float* float_ptr = reinterpret_cast<float*>(number); float_ptr[0] == 50.0f) {
                std::cout << "number[0] == 50.0f\n";
            }
        } else if constexpr (std::is_same_v<T, __m512>) {
            if (const auto* float_ptr = reinterpret_cast<float*>(number); float_ptr[0] == 50.0f) {
                std::cout << "number[0] == 50.0f\n";
            }
#endif

        } else {
            if (*number == 50) {
                std::cout <<"number == 50\n";
            }
        }
    }

public:
    static constexpr long long get_array_size() {
        return m_array_size;
    }

    static void measure_read(T* array) {
        T sum {};
        auto read_iteration = [&](int i) {
            if constexpr(std::is_same_v<T, __m128>) {
                sum = _mm_add_ps(
                    sum,
                    _mm_load_ps(reinterpret_cast<const float*>(&array[i]))
                );
#ifndef SERVER
            } else if constexpr (std::is_same_v<T, __m256>) {
                sum = _mm256_add_ps(sum,_mm256_load_ps(reinterpret_cast<const float*>(&array[i])));
            } else if constexpr (std::is_same_v<T, __m512>) {
                sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i])));
#endif
            } else { sum += array[i]; }
        };
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; i += UNROLL_SIZE) {
            // read_iteration(i);
            // read_iteration(i + 1);
            // read_iteration(i + 2);
            // read_iteration(i + 3);
            // read_iteration(i + 4);
            // read_iteration(i + 5);
            // read_iteration(i + 6);
            // read_iteration(i + 7);

            if constexpr(std::is_same_v<T, __m128>) {
                sum = _mm_add_ps(sum,_mm_load_ps(reinterpret_cast<const float*>(&array[i])));
#ifndef SERVER
            } else if constexpr (std::is_same_v<T, __m256>) {
                sum = _mm256_add_ps(sum,_mm256_load_ps(reinterpret_cast<const float*>(&array[i])));
            } else if constexpr (std::is_same_v<T, __m512>) {
                sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i])));
                // sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i + 1])));
                // sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i + 2])));
                // sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i + 3])));
                // sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i + 4])));
                // sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i + 5])));
                // sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i + 6])));
                // sum = _mm512_add_ps(sum,_mm512_load_ps(reinterpret_cast<const float*>(&array[i + 7])));
#endif

            } else {
                sum += array[i];
            }
        }
        const auto end = std::chrono::high_resolution_clock::now();

        check_number(&sum);
        report_performance("Read", start, end);
    }

    static void measure_write(T* array) {
        // measure_memset(array);
        // return;
        auto write_iteration = [&](int i) {
            if constexpr (std::is_same_v<T, __m128>) {
                _mm_store_ps(reinterpret_cast<float*>(&array[i]), _mm_set1_ps(1.0f));
#ifndef SERVER
            } else if constexpr (std::is_same_v<T, __m256>) {
                _mm256_store_ps(reinterpret_cast<float*>(&array[i]), _mm256_set1_ps(1.0f));
            } else if constexpr (std::is_same_v<T, __m512>) {
                _mm512_store_ps(reinterpret_cast<float*>(&array[i]), _mm512_set1_ps(1.0f));
#endif

            } else {
                array[i] = 1;
            }
        };
        const __m512 set1 = _mm512_set1_ps(1.0f);
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; i += UNROLL_SIZE) {
            // write_iteration(i);
            // write_iteration(i + 1);
            // write_iteration(i + 2);
            // write_iteration(i + 3);
            // write_iteration(i + 4);
            // write_iteration(i + 5);
            // write_iteration(i + 6);
            // write_iteration(i + 7);

            if constexpr (std::is_same_v<T, __m128>) {
                _mm_store_ps(reinterpret_cast<float*>(&array[i]), _mm_set1_ps(1.0f));
#ifndef SERVER
            } else if constexpr (std::is_same_v<T, __m256>) {
                _mm256_store_ps(reinterpret_cast<float*>(&array[i]), _mm256_set1_ps(1.0f));
            } else if constexpr (std::is_same_v<T, __m512>) {
                _mm512_stream_ps(reinterpret_cast<float*>(&array[i]), _mm512_set1_ps(1.0f));
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i]), set1);
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i + 1]), set1);
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i + 2]), set1);
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i + 3]), set1);
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i + 4]), set1);
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i + 5]), set1);
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i + 6]), set1);
                // _mm512_store_ps(reinterpret_cast<float*>(&array[i + 7]), set1);
#endif

            } else {
                array[i] = 1;
            }
        }
        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&array[20]);

        report_performance("Write", start, end);
    }

    static void measure_replace(T* array1, T* array2) {
        auto replace_iteration = [&](int i) {
            if constexpr (std::is_same_v<T, __m128>) {
                _mm_store_ps(reinterpret_cast<float*>(&array1[i]),
               _mm_load_ps(reinterpret_cast<const float*>(&array2[i])));
#ifndef SERVER
            } else if constexpr (std::is_same_v<T, __m256>) {
                _mm256_store_ps(reinterpret_cast<float*>(&array1[i]),
                    _mm256_load_ps(reinterpret_cast<const float*>(&array2[i])));
            } else if constexpr (std::is_same_v<T, __m512>) {
                _mm512_store_ps(reinterpret_cast<float*>(&array1[i]),
                    _mm512_load_ps(reinterpret_cast<const float*>(&array2[i])));
#endif
            } else {
                array1[i] = array2[i];
            }
        };
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i {}; i < m_array_size; i += UNROLL_SIZE) {
            // replace_iteration(i);
            // replace_iteration(i + 1);
            // replace_iteration(i + 2);
            // replace_iteration(i + 3);
            // replace_iteration(i + 4);
            // replace_iteration(i + 5);
            // replace_iteration(i + 6);
            // replace_iteration(i + 7);

             if constexpr (std::is_same_v<T, __m128>) {
                 _mm_store_ps(reinterpret_cast<float*>(&array1[i]),
                _mm_load_ps(reinterpret_cast<const float*>(&array2[i])));
#ifndef SERVER
             } else if constexpr (std::is_same_v<T, __m256>) {
                _mm256_store_ps(reinterpret_cast<float*>(&array1[i]),
                    _mm256_load_ps(reinterpret_cast<const float*>(&array2[i])));
            } else if constexpr (std::is_same_v<T, __m512>) {
                _mm512_store_ps(reinterpret_cast<float*>(&array1[i]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i])));
                // _mm512_store_ps(reinterpret_cast<float*>(&array1[i + 1]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i + 1])));
                // _mm512_store_ps(reinterpret_cast<float*>(&array1[i + 2]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i + 2])));
                // _mm512_store_ps(reinterpret_cast<float*>(&array1[i + 3]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i + 3])));
                // _mm512_store_ps(reinterpret_cast<float*>(&array1[i + 4]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i + 4])));
                // _mm512_store_ps(reinterpret_cast<float*>(&array1[i + 5]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i + 5])));
                // _mm512_store_ps(reinterpret_cast<float*>(&array1[i + 6]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i + 6])));
                // _mm512_store_ps(reinterpret_cast<float*>(&array1[i + 7]),_mm512_load_ps(reinterpret_cast<const float*>(&array2[i + 7])));
#endif
            } else {
                array1[i] = array2[i];
            }
        }
        const auto end = std::chrono::high_resolution_clock::now();

        check_number(&array1[2]);
        check_number(&array2[2]);
        report_performance("Replace", start, end);
    }

    static void measure_nt_write() {
        std::vector<T> vector (get_array_size());
        T* array = vector.data();
        const auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < m_array_size; ++i) {
            if constexpr (std::is_same_v<T, __m512>) {
                // Non-temporal store - bypasses cache
                _mm512_stream_ps(reinterpret_cast<float*>(&array[i]), _mm512_set1_ps(1.0f));
            } else {
                // Fallback for non-AVX512 types
                array[i] = T{1};
            }
        }
        _mm_sfence(); // Ensure stores are complete

        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&array[20]);
        report_performance("NT_Write", start, end);
    }

    static void measure_nt_read() {
        std::vector<T> vector (get_array_size());
        T* array = vector.data();
        T sum{};
        const auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < m_array_size; ++i) {
            if constexpr (std::is_same_v<T, __m512>) {
                // Non-temporal load - bypasses cache
                __m512i temp = _mm512_stream_load_si512(reinterpret_cast<const __m512i*>(&array[i]));
                sum = _mm512_add_ps(sum, _mm512_castsi512_ps(temp));
            } else {
                sum += array[i];
            }
        }

        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&sum);
        report_performance("NT_Read", start, end);
    }

    static void measure_nt_copy() {
        std::vector<T> vector1 (get_array_size());
        std::vector<T> vector2 (get_array_size());
        T* dst = vector1.data();
        T* src = vector2.data();
        const auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < m_array_size; ++i) {
            if constexpr (std::is_same_v<T, __m512>) {
                // Non-temporal load + store combination
                __m512i temp = _mm512_stream_load_si512((&src[i]));
                _mm512_stream_ps(reinterpret_cast<float*>(&dst[i]), _mm512_castsi512_ps(temp));
            } else {
                dst[i] = src[i];
            }
        }
        _mm_sfence(); // Ensure all stores are complete

        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&dst[20]);
        report_performance("NT_Copy", start, end);
    }

    static void test_for_type() {

        {
            std::cout << demangle_type_name() << ":" << std::endl;
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<T> vector1 (get_array_size());
                    MemoryBenchmark::measure_read(vector1.data());

                }
            }
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<T> vector1 (get_array_size());
                    MemoryBenchmark::measure_write(vector1.data());

                }
            }
            for (int i {}; i < TEST_TIMES; ++i) {
                {
                    std::vector<T> vector1 (get_array_size());
                    std::vector<T> vector2 (get_array_size());
                    MemoryBenchmark::measure_replace(vector1.data(), vector2.data());
                }
            }
            std::cout << "=========================\n" << std::endl;
        }
    }

static void test_for_type_threads(int num_threads) {
#ifndef SERVER
    std::cout << demangle_type_name() << " with " << num_threads << " threads:" << std::endl;

    std::vector<double> thread_bandwidths(num_threads);
    std::barrier sync_point(num_threads);


    auto pin_thread = [](int core_id) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    };

    auto thread_func = [&](int thread_id, const std::string& op_name, auto operation) {
        pin_thread(thread_id);

        // Each thread allocates its own memory
        const size_t chunk_size = m_array_size / num_threads;
        std::vector<__m512> local_data(chunk_size);
        std::vector<__m512> local_data2; // For copy operations
        if (op_name == "Copy") {
            local_data2.resize(chunk_size);
        }

        operation(local_data.data(), local_data2.data());

        sync_point.arrive_and_wait();

        const auto start = std::chrono::high_resolution_clock::now();
        operation(local_data.data(), local_data2.data());
        const auto end = std::chrono::high_resolution_clock::now();

        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        const double time_spent = static_cast<double>(duration.count()) * 1e-6;
        const double gb_per_second = static_cast<double>(chunk_size * sizeof(__m512)) / GB / time_spent;
        thread_bandwidths[thread_id] = gb_per_second;

        sync_point.arrive_and_wait();
    };

    {
        std::cout << "\nTesting parallel read:\n";
        for (int test = 0; test < TEST_TIMES; ++test) {
            std::vector<std::thread> threads;
            std::ranges::fill(thread_bandwidths, 0.0);

            auto read_op = [thread_bandwidths](__m512* data, __m512*) {
                __m512 sum = _mm512_setzero_ps();
                const size_t size = m_array_size / thread_bandwidths.size();
                for (size_t i = 0; i < size; ++i) {
                    sum = _mm512_add_ps(sum, reinterpret_cast<__m512>(_mm512_stream_load_si512(&data[i])));
                }
                check_number(&sum);
            };

            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(thread_func, i, "Read", read_op);
            }

            for (auto& thread : threads) {
                thread.join();
            }

            const double total_bandwidth = std::accumulate(thread_bandwidths.begin(), thread_bandwidths.end(), 0.0);
            std::cout << "Total Read Bandwidth: " << total_bandwidth << " GB/S\n";
        }
    }

    {
        std::cout << "\nTesting parallel write:\n";
        for (int test = 0; test < TEST_TIMES; ++test) {
            std::vector<std::thread> threads;
            std::ranges::fill(thread_bandwidths, 0.0);

            auto write_op = [thread_bandwidths](__m512* data, __m512*) {
                const __m512 value = _mm512_set1_ps(1.0f);
                const size_t size = m_array_size / thread_bandwidths.size();
                for (size_t i = 0; i < size; ++i) {
                    _mm512_stream_ps(reinterpret_cast<float*>(&data[i]), value);
                }
                _mm_sfence();
            };

            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(thread_func, i, "Write", write_op);
            }

            for (auto& thread : threads) {
                thread.join();
            }

            const double total_bandwidth = std::accumulate(thread_bandwidths.begin(), thread_bandwidths.end(), 0.0);
            std::cout << "Total Write Bandwidth: " << total_bandwidth << " GB/S\n";
        }
    }

    {
        std::cout << "\nTesting parallel copy:\n";
        for (int test = 0; test < TEST_TIMES; ++test) {
            std::vector<std::thread> threads;
            std::ranges::fill(thread_bandwidths, 0.0);

            auto copy_op = [thread_bandwidths](__m512* dst, __m512* src) {
                const size_t size = m_array_size / thread_bandwidths.size();
                for (size_t i = 0; i < size; ++i) {
                    const __m512i temp = _mm512_stream_load_si512(&src[i]);
                    _mm512_stream_ps(reinterpret_cast<float*>(&dst[i]), _mm512_castsi512_ps(temp));
                }
                _mm_sfence();
            };

            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(thread_func, i, "Copy", copy_op);
            }

            for (auto& thread : threads) {
                thread.join();
            }

            const double total_bandwidth = std::accumulate(thread_bandwidths.begin(), thread_bandwidths.end(), 0.0);
            std::cout << "Total Copy Bandwidth: " << total_bandwidth << " GB/S\n";
        }
    }

    std::cout << "=========================\n" << std::endl;
#endif
#ifdef SERVER
std::cout << demangle_type_name() << " with " << num_threads << " threads:" << std::endl;

    std::vector<double> thread_bandwidths(num_threads);
    // std::barrier sync_point(num_threads);


    auto pin_thread = [](int core_id) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    };

    auto thread_func = [&](int thread_id, const std::string& op_name, auto operation) {
        pin_thread(thread_id);

        // Each thread allocates its own memory
        const size_t chunk_size = m_array_size / num_threads;
        std::vector<__m128> local_data(chunk_size);
        std::vector<__m128> local_data2; // For copy operations
        if (op_name == "Copy") {
            local_data2.resize(chunk_size);
        }

        operation(local_data.data(), local_data2.data());

        // sync_point.arrive_and_wait();

        const auto start = std::chrono::high_resolution_clock::now();
        operation(local_data.data(), local_data2.data());
        const auto end = std::chrono::high_resolution_clock::now();

        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        const double time_spent = static_cast<double>(duration.count()) * 1e-6;
        const double gb_per_second = static_cast<double>(chunk_size * sizeof(__m128)) / GB / time_spent;
        thread_bandwidths[thread_id] = gb_per_second;

        // sync_point.arrive_and_wait();
    };
/*
    {
        std::cout << "\nTesting parallel read:\n";
        for (int test = 0; test < TEST_TIMES; ++test) {
            std::vector<std::thread> threads;
            std::ranges::fill(thread_bandwidths, 0.0);

            auto read_op = [thread_bandwidths](__m128* data, __m128*) {
                __m128 sum = _mm_setzero_ps();
                const size_t size = m_array_size / thread_bandwidths.size();
                for (size_t i = 0; i < size; ++i) {
                    sum = _mm_add_ps(sum, (_mm_stream_load_si128(&data[i])));
                }
                check_number(&sum);
            };

            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(thread_func, i, "Read", read_op);
            }

            for (auto& thread : threads) {
                thread.join();
            }

            const double total_bandwidth = std::accumulate(thread_bandwidths.begin(), thread_bandwidths.end(), 0.0);
            std::cout << "Total Read Bandwidth: " << total_bandwidth << " GB/S\n";
        }
    }
*/
    {
        std::cout << "\nTesting parallel write:\n";
        for (int test = 0; test < TEST_TIMES; ++test) {
            std::vector<std::thread> threads;
            std::ranges::fill(thread_bandwidths, 0.0);

            auto write_op = [thread_bandwidths](__m128* data, __m128*) {
                const __m128 value = _mm_set1_ps(1.0f);
                const size_t size = m_array_size / thread_bandwidths.size();
                for (size_t i = 0; i < size; ++i) {
                    _mm_stream_ps(reinterpret_cast<float*>(&data[i]), value);
                }
                _mm_sfence();
            };

            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(thread_func, i, "Write", write_op);
            }

            for (auto& thread : threads) {
                thread.join();
            }

            const double total_bandwidth = std::accumulate(thread_bandwidths.begin(), thread_bandwidths.end(), 0.0);
            std::cout << "Total Write Bandwidth: " << total_bandwidth << " GB/S\n";
        }
    }
/*
    {
        std::cout << "\nTesting parallel copy:\n";
        for (int test = 0; test < TEST_TIMES; ++test) {
            std::vector<std::thread> threads;
            std::ranges::fill(thread_bandwidths, 0.0);

            auto copy_op = [thread_bandwidths](__m128* dst, __m128i* src) {
                const size_t size = m_array_size / thread_bandwidths.size();
                for (size_t i = 0; i < size; ++i) {
                    const __m128i temp = _mm_stream_load_si128(&src[i]);
                    _mm512_stream_ps(reinterpret_cast<float*>(&dst[i]), _mm_castsi128_ps(temp));
                }
                _mm_sfence();
            };

            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(thread_func, i, "Copy", copy_op);
            }

            for (auto& thread : threads) {
                thread.join();
            }

            const double total_bandwidth = std::accumulate(thread_bandwidths.begin(), thread_bandwidths.end(), 0.0);
            std::cout << "Total Copy Bandwidth: " << total_bandwidth << " GB/S\n";
        }
    }
*/
    std::cout << "=========================\n" << std::endl;
#endif
}


    static void measure_memset(T* array) {
        const auto start = std::chrono::high_resolution_clock::now();

        memset(array, 1, m_array_size * sizeof(T));

        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&array[20]);
        report_performance("Memset", start, end);
    }

    static void measure_memcpy(T* dst, const T* src) {
        const auto start = std::chrono::high_resolution_clock::now();

        memcpy(dst, src, m_array_size * sizeof(T));

        const auto end = std::chrono::high_resolution_clock::now();
        check_number(&dst[20]);
        report_performance("Memcpy", start, end);
    }

    static void measure_memchr(T* array, T value) {
        const auto start = std::chrono::high_resolution_clock::now();

        void* result = memchr(array, value, m_array_size * sizeof(T));

        const auto end = std::chrono::high_resolution_clock::now();
        if (result) {
            check_number(static_cast<T*>(result));
        }
        report_performance("Memchr", start, end);
    }


    static void test_lib_functions() {
        std::cout << "\nTesting lib functions:\n";

        for (int i = 0; i < TEST_TIMES; ++i) {
            {
                std::vector<T> vector(get_array_size());
                measure_memset(vector.data());
            }
        }

        for (int i = 0; i < TEST_TIMES; ++i) {
            {
                std::vector<T> src(get_array_size());
                std::vector<T> dst(get_array_size());
                measure_memcpy(dst.data(), src.data());
            }
        }

        for (int i = 0; i < TEST_TIMES; ++i) {
            {
                std::vector<T> vector(get_array_size());
                // Устанавливаем искомое значение
                vector[vector.size() - 2] = T{50};
                measure_memchr(vector.data(), T{50});
            }
        }

        std::cout << "=========================\n";
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
