#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>

// #include <cilk/cilk.h>
// #include <cilk/cilk_api.h>

// #define parallel_for cilk_for
// #define parallel_spawn cilk_spawn
// #define parallel_sync cilk_sync

using namespace std;

typedef void (*fun_ptr)(int32_t *data, uint32_t size);

class Array;

bool parse_args(int argc, char **argv);
bool initialize();
void print_data_info();

