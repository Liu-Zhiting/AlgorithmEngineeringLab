#pragma once
#include <iostream>
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

typedef struct Node
{
    int32_t value;
    struct Node *next;
} Node;
typedef Node *Linklist;

class AdjointList;

class Solution
{
private:
    void initialize();
    void dispose();

public:
    uint32_t size;
    uint32_t *distance;
    Solution() : size(0), distance(nullptr){};
    Solution(uint32_t size) : distance(nullptr) { initialize(); };
    Solution(const Solution &other);
    ~Solution() { dispose(); };

    void attach_to_graph(const AdjointList &graph);
    void clear() { memset(distance, 0, size * sizeof(uint32_t)); };

    bool operator==(const Solution &other) const;
    Solution &operator=(const Solution &other);
};

typedef void (*fun_ptr)(const AdjointList &, Solution &);

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

const int ROOT_ID = 0;