#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

class Matrix;
typedef void (*fun_ptr)(
    const int32_t *const *const a,
    const int32_t *const *const b,
    int32_t *const *const c,
    const uint32_t size);

bool parse_args(int argc, char **argv);
bool initialize();
void print_data_info();


