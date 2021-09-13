#pragma once
#include "utils.hpp"
#include <iomanip>
#include <chrono>
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

struct TestCase
{
    string name;
    fun_ptr func;
};

struct TestResult
{
    string name;
    double time;
    bool correctness;
};

TestResult run_and_measure_time(TestCase);
vector<TestResult> test_all();
void dump_result(vector<TestResult> &cases);

void ref(int32_t* data,uint32_t size);
void merge_sort(int32_t* data,uint32_t size);
void quick_sort(int32_t* data,uint32_t size);