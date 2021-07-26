#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <sstream>
#include "adjoint_list.hpp"

#include "cilk/cilk.h"

#define parallel_for cilk_for
#define parallel_spawn cilk_spawn
#define parallel_sync cilk_sync

using namespace std;