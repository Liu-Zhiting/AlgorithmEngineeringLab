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

class MatArray;
typedef string (*fun_ptr)(const MatArray& A, const MatArray& B, MatArray& result);
struct TestCase
{
    string name;
    int nworkers;
    double time;
    bool correctness;
    fun_ptr func;
};
typedef struct TestCase TestCase;

bool initialize(int argc, char** argv);
TestCase run_and_measure_time(fun_ptr func);
vector<TestCase> test_all();
void dump_result(vector<TestCase>& cases);


