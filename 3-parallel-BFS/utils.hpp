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

typedef struct Node
{
    int32_t value;
    struct Node *next;
} Node;
typedef Node *Linklist;

class AdjointList
{
private:
    void initialize();
    void dispose();

public:
    uint32_t vertex_count;
    uint32_t *out_degree;
    Node *vertex;

    AdjointList() : vertex_count(0), out_degree(nullptr), vertex(nullptr){};
    AdjointList(int vertex_count) : vertex_count(vertex_count) { initialize(); };
    AdjointList(const AdjointList &other);
    AdjointList(const char *binary_filename);
    ~AdjointList() { dispose(); };

    bool operator==(const AdjointList &other) const;
    AdjointList &operator=(const AdjointList &other);

    uint32_t get_vertex_count() { return this->vertex_count; };
    void add_edge(int from, int to);
    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);

    void save_file_binary(const char *binary_filename);
    void dump_adjoint_list();
};
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

    bool operator==(const Solution &other) const;
    Solution &operator=(const Solution &other);
};

typedef string (*fun_ptr)(const Graph &graph, Solution &solution);
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

string ref(const Graph &graph, Solution &solution);
