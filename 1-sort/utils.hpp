#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#define parallel_for cilk_for
#define parallel_spawn cilk_spawn
#define parallel_sync cilk_sync

using namespace std;
using chrono::duration;
using chrono::duration_cast;
using chrono::high_resolution_clock;

typedef void (*fun_ptr)(int32_t *data, uint32_t size);

class Array
{
private:
    void dispose();
    int32_t *data;
    uint32_t size;
public:
    Array() : data(nullptr), size(0){};
    Array(uint32_t size) : size(size) { data = new int32_t[size]; };
    Array(const Array &other);
    ~Array() { dispose(); };
    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    bool operator==(const Array &other) const;
    Array &operator=(const Array &other);
    void sort_with_func(fun_ptr func) { func(data, size); };
};

typedef struct TestCase
{
    string name;
    fun_ptr func;
} TestCase;

typedef struct TestResult
{
    string name;
    int nworkers;
    double time;
    bool correctness;
} TestResult;

bool initialize(int argc, char **argv);
TestResult run_and_measure_time(TestCase);
vector<TestResult> test_all();
void dump_result(vector<TestResult> &cases);

void ref_sort(int32_t* data,uint32_t size);
void merge_sort(int32_t* data,uint32_t size);
void quick_sort(int32_t* data,uint32_t size);
