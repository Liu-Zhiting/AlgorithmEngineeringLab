#include "utils.hpp"
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#define parallel_for cilk_for
#define parallel_spawn cilk_spawn
#define parallel_sync cilk_sync

void par_ref(
    const int32_t *const *const a,
    const int32_t *const *const b,
    int32_t *const *const c,
    const uint32_t size)
{
    parallel_for (int i = 0; i < size; i++)
        parallel_for (int k = 0; k < size; k++)
            for(int j = 0; j < size; j++)
                c[i][j] += a[i][k] * b[k][j];
}