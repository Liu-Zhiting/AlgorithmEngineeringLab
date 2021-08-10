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

class AdjointList;
using Graph = AdjointList;

typedef uint32_t (*fun_ptr)(const Graph &graph);

bool parse_args(int argc, char **argv);
bool initialize();
void print_data_info();

