#pragma once
#include "utils.hpp"
#include <iomanip>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

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

uint32_t ref(const Graph &graph);
uint32_t tc_merge(const Graph &graph);
uint32_t tc_hash(const Graph &graph);
uint32_t tc_matrix(const Graph &graph);