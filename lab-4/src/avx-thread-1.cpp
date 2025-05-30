#include <chrono>
#include <format>
#include <immintrin.h>
#include <iostream>
#include <vector>
#include <sys/time.h>

constexpr int DATA_SIZE {8};
constexpr int ARRAY_SIZE {1073741824 / DATA_SIZE};
constexpr int MEAS_COUNT {10000000};

#define SAVE_FOR(x) \
        if (x == 50) { \
            printf("x == 50"); \
        }

void prepare_processor() {
    double  x = 2;
    for (int i {}; i < MEAS_COUNT * 200; ++i) {
        x *= 2;
    }
    if (x == 13432) {
        std::cout << std::format("x = {}\n", x);
    }
}

/*
void* wcalloc(size_t size) {
    void* mem = malloc(size);
    if (mem == NULL) {
        printf("wcalloc: malloc error%s", strerror(errno));
        abort();
    }
    memset(mem, 1, size);
    return mem;
}
*/

void measure_read(const float* array) {
    __m512 sum_vec = _mm512_setzero_ps();

    const auto start = std::chrono::high_resolution_clock::now();
    for (int i {}; i < ARRAY_SIZE; i += 8) {
        const __m512 data = _mm512_loadu_ps(array + i);
        sum_vec = _mm512_add_ps(sum_vec, data);
    }
    const auto end = std::chrono::high_resolution_clock::now();

    if (sum_vec[0] == 50) {
        printf("sum_vec[0] == 50\n");
    }

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    const double time_spent = static_cast<double>(duration.count()) * 1e-6;
    const double gb_per_second = static_cast<double>(ARRAY_SIZE * DATA_SIZE) / 1024 / 1024 / 1024 / time_spent;
    std::cout << std::format ("Read: {} GB/S \n", gb_per_second);
}

void measure_write(float* array) {

    const __m512 scale_1_vec = _mm512_set1_ps(1);

    const auto start = std::chrono::high_resolution_clock::now();
    for (int i {}; i < ARRAY_SIZE; i += 8) {
        _mm512_storeu_ps(array + i , scale_1_vec);
    }
    const auto end = std::chrono::high_resolution_clock::now();

    if (array[0] == 50) {
        printf("array[0] == 50\n");
    }

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    const double time_spent = static_cast<double>(duration.count()) * 1e-6;
    const double gb_per_second = static_cast<double>(ARRAY_SIZE * DATA_SIZE) / 1024 / 1024 / 1024 / time_spent;
    std::cout << std::format ("Write: {} GB/S \n", gb_per_second);
}

void measure_replace(const float* array1, float* array2) {
    const auto start = std::chrono::high_resolution_clock::now();
    for (int i {}; i < ARRAY_SIZE; i += 8) {
        const __m512 res = _mm512_loadu_ps(array1 + i);
        _mm512_storeu_ps(array2 + i , res);
    }
    const auto end = std::chrono::high_resolution_clock::now();
    SAVE_FOR(array2[3]);
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    const double time_spent = static_cast<double>(duration.count()) * 1e-6;
    const double gb_per_second = static_cast<double>(ARRAY_SIZE * DATA_SIZE) / 1024 / 1024 / 1024 / time_spent;
    std::cout << std::format ("Replace: {} GB/S \n", gb_per_second);
}


int main() {
    prepare_processor();
    std::vector vector1 (ARRAY_SIZE * DATA_SIZE, static_cast<std::byte>(1));
    std::vector vector2 (ARRAY_SIZE * DATA_SIZE, static_cast<std::byte>(1));

    measure_read(arr1);
    measure_write(arr1);
    measure_replace(arr1, arr2);
    return 0;
}