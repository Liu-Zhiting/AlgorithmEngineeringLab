#pragma once
#include "utils.hpp"
#include <iomanip>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

string ref(const Matrix& A, const Matrix& B, Matrix& result);
string loop(const Matrix& A, const Matrix& B, Matrix& result);
string divide_and_conquer(const Matrix& A, const Matrix& B, Matrix& result);
string strassen(const Matrix& A, const Matrix& B, Matrix& result);