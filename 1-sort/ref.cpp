#include "utils.hpp"
#include <cstdlib>

int cmp_int32(const void *a, const void *b)
{
    return (*(int32_t *)a - *(int32_t *)b);
}

void ref(int32_t *data, uint32_t size)
{
    qsort(data, size, sizeof(int32_t), cmp_int32);
}