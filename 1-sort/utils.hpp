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

#ifdef _CILK_H
#define parallel_for cilk_for
#define parallel_spawn cilk_spawn
#define parallel_sync cilk_sync
#else
#define parallel_for for
#define parallel_spawn
#define parallel_sync
#endif

using namespace std;
using chrono::duration;
using chrono::duration_cast;
using chrono::high_resolution_clock;

class Array
{
private:
    void dispose();

public:
    int32_t *data;
    uint32_t size;

    Array() : data(nullptr), size(0){};
    Array(uint32_t size) : size(size) { data = new int32_t[size]; };
    Array(const Array &other);
    ~Array() { dispose(); };
    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    bool operator==(const Array &other) const;
    Array &operator=(const Array &other);
};

typedef string (*fun_ptr)(Array &array);
typedef struct TestCase
{
    string name;
    int nworkers;
    double time;
    bool correctness;
    fun_ptr func;
} TestCase;

bool initialize(int argc, char **argv);
TestCase run_and_measure_time(fun_ptr func);
vector<TestCase> test_all();
void dump_result(vector<TestCase> &cases);

string ref_sort(Array &array);
string merge_sort(Array &array);
string quick_sort(Array &array);
