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
using chrono::high_resolution_clock;
using chrono::duration;
using chrono::duration_cast;

typedef struct SortArray
{
    int32_t* data;
    uint32_t size;

    SortArray():data(nullptr),size(0){};
    SortArray(uint32_t size):size(size){data=new int32_t[size];};
    SortArray(const SortArray& other);
    bool load_data_text(const char* filename);
    bool load_data_binary(const char* filename);
    bool operator==(const SortArray& other) const;

} SortArray;
typedef string (*fun_ptr)(SortArray& array);
typedef struct TestCase
{
    string name;
    int nworkers;
    double time;
    bool correctness;
    fun_ptr func;
} TestCase;

bool initialize(int argc, char** argv);
TestCase run_and_measure_time(fun_ptr func);
vector<TestCase> test_all();
void dump_result(vector<TestCase>& cases);

string ref(SortArray& array);


