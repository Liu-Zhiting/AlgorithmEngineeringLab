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

class Matrix;
typedef void (*fun_ptr)(
    const int32_t *const *const a,
    const int32_t *const *const b,
    int32_t *const *const c,
    const uint32_t size);

bool initialize(int argc, char **argv);


