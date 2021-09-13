#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

class StaticAdjointList;
using Graph = StaticAdjointList;

typedef uint32_t (*fun_ptr)(const Graph &graph);

bool parse_args(int argc, char **argv);
bool initialize();
void print_data_info();

