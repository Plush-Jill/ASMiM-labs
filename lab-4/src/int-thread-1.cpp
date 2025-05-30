#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define DATA_SIZE sizeof(int)
#define ARRAY_SIZE (size_t)4 * 1073741824 / DATA_SIZE
#define MEAS_COUNT 10000000

#define SAVE_FOR(x) \
        if (x == 50) { \
            printf("x == 50"); \
        }

void prepare_processor() {
    double  x = 2;
    for (register size_t i = 0; i < (size_t)MEAS_COUNT * 200; ++i) {
        x *= 2;
    }
    SAVE_FOR(x);
}


void* wcalloc(size_t size) {
    void* mem = malloc(size);
    if (mem == NULL) {
        printf("wcalloc: malloc error%s", strerror(errno));
        abort();
    }
    memset(mem, 1, size);
    return mem;
}

void read_meas(int* arr) {
    int sum = 0;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        sum += arr[i];
    }
    gettimeofday(&end, NULL);
    SAVE_FOR(sum);
    SAVE_FOR(arr[3]);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double time_spent = seconds + microseconds * 1e-6;
    double GB_S = (double)(ARRAY_SIZE * DATA_SIZE) / 1024 / 1024 / 1024 / time_spent;
    printf("read: %.12lf GB/S \n", GB_S);
}

void write_meas(int* arr) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = 1;
    }
    gettimeofday(&end, NULL);
    SAVE_FOR(arr[3]);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double time_spent = seconds + microseconds * 1e-6;
    double GB_S = (double)(ARRAY_SIZE * DATA_SIZE) / 1024 / 1024 / 1024 / time_spent;
    printf("write: %.12lf GB/S \n", GB_S);
}

void replace_meas(int* arr1, int* arr2) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr1[i] = arr2[i];
    }
    gettimeofday(&end, NULL);
    SAVE_FOR(arr1[3]);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double time_spent = seconds + microseconds * 1e-6;
    double GB_S = (double)(ARRAY_SIZE * DATA_SIZE) / 1024 / 1024 / 1024 / time_spent;
    printf("replace: %.12lf GB/S \n", GB_S);
}


int main() {
    prepare_processor();
    int* arr1 = wcalloc(ARRAY_SIZE * DATA_SIZE);
    int* arr2 = wcalloc(ARRAY_SIZE * DATA_SIZE);
    read_meas(arr1);
    write_meas(arr1);
    replace_meas(arr1, arr2);
    return 0;
