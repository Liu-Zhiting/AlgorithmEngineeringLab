#pragma once
#include "utils.hpp"
#include <iomanip>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

void ref(const int32_t *const *const a, const int32_t *const *const b, int32_t *const *const c, const uint32_t size);
void loop(const int32_t *const *const a, const int32_t *const *const b, int32_t *const *const c, const uint32_t size);
void divide_and_conquer(const int32_t *const *const a, const int32_t *const *const b, int32_t *const *const c, const uint32_t size);
void strassen(const int32_t *const *const a, const int32_t *const *const b, int32_t *const *const c, const uint32_t size);