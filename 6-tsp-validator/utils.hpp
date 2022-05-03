#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>
#include <exception>

using namespace std;

bool parse_args(int argc, char **argv);
bool initialize();
void print_data_info();

void runCmp();
void printDataSetNames();

