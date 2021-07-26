#pragma once

#include <iostream>
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

class Matrix;
typedef void (*fun_ptr)(const Matrix& A, const Matrix& B, Matrix& result);

typedef struct TestCase
{
    string name;
    fun_ptr func;
} TestCase;

struct TestResult
{
    string name;
    int nworkers;
    double time;
    bool correctness;
};
typedef struct TestResult TestResult;

bool initialize(int argc, char** argv);
TestResult run_and_measure_time(TestCase);
vector<TestResult> test_all();
void dump_result(vector<TestResult>& cases);


