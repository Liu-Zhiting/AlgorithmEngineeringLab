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

typedef struct Node
{
    uint32_t value;
    struct Node *next;
} Node;
typedef Node *Linklist;

class AdjointList;
typedef AdjointList Graph;

class Solution
{
private:
    void initialize();
    void dispose();

public:
    uint32_t size;
    uint32_t* distance;
    Solution():size(0),distance(nullptr){};
    Solution(uint32_t size){initialize();};
    Solution(const Solution& other);
    ~Solution(){ dispose();};

    void attach_to_graph(const Graph& graph);
    void clear() { memset(distance, 0, size * sizeof(uint32_t)); };

    bool operator==(const Solution &other) const;
    Solution &operator=(const Solution &other);
};

typedef string (*fun_ptr)(const Graph &graph, uint32_t& result);
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

string ref(const Graph &graph, uint32_t& result);
