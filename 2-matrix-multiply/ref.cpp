#include "utils.hpp"

void ref(
    const int32_t *const *const a,
    const int32_t *const *const b,
    int32_t *const *const c,
    const uint32_t size)
{
    for (int i = 0; i < size; i++)
        for (int k = 0; k < size; k++)
            for(int j = 0; j < size; j++)
                c[i][j] += a[i][k] * b[k][j];
}