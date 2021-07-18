#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <sstream>
#include "adjoint_list.hpp"

#include "cilk/cilk.h"

#ifdef _CILK_H
#define parallel_for cilk_for
#define parallel_spawn cilk_spawn
#define parallel_sync cilk_sync
#else
#define parallel_for for
#define parallel_spawn
#define parallel_sync
#endif

using namespace std;