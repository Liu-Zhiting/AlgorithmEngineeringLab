#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

class AdjointList;
using Graph = AdjointList;
class Solution;
typedef void (*fun_ptr)(const AdjointList &, Solution &);

bool parse_args(int argc, char **argv);
bool initialize();
void print_data_info();

const int ROOT_ID = 0;