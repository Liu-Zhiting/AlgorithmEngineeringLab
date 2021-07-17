#pragma once
#include "utils.hpp"
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;



string ref(const MatArray& A, const MatArray& B, MatArray& result);
string loop(const MatArray& A, const MatArray& B, MatArray& result);
string parallel_d2c(const MatArray& A, const MatArray& B, MatArray& result);
string strassen(const MatArray& A, const MatArray& B, MatArray& result);