#pragma once
#include "utils.hpp"
#include <iomanip>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

void ref(const Matrix& A, const Matrix& B, Matrix& result);
void loop(const Matrix& A, const Matrix& B, Matrix& result);
void divide_and_conquer(const Matrix& A, const Matrix& B, Matrix& result);
void strassen(const Matrix& A, const Matrix& B, Matrix& result);