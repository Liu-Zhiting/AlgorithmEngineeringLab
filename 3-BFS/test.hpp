#pragma once
#include "utils.hpp"
#include <iomanip>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

void ref_BFS(const AdjointList &graph, Solution &solution);
void buttom_up(const AdjointList &graph, Solution &solution);
