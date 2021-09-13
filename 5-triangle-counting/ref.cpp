#include "utils.hpp"
#include "static_adjoint_list.hpp"

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#define parallel_for cilk_for
#define parallel_spawn cilk_spawn
#define parallel_sync cilk_sync

uint32_t ref_get_par_sum(const uint32_t *const array, long length)
{
    uint32_t result = 0.0;
    if (length <= 256)
    {
        for (long i = 0; i < length; i++)
            result += array[i];
        return result;
    }
    uint32_t x, y;
    parallel_spawn x = ref_get_par_sum(array, length / 2);
    y = ref_get_par_sum((array + length / 2), length - length / 2);
    parallel_sync;
    result = x + y;
    return result;
}

uint32_t ref(const Graph &graph)
{
    const uint32_t n = graph.get_vertex_count();
    uint32_t result = 0;
    uint32_t *tmp_result = new uint32_t[n * n];
    uint32_t **A = graph.convert_to_adjoint_matrix();
    uint32_t **A2 = new uint32_t *[n];
    for (int i = 0; i < n; i++)
    {
        A2[i] = new uint32_t[n];
        memset(A2[i], 0, n * sizeof(uint32_t));
    }

    parallel_for(int i = 0; i < n; i++)
        parallel_for(int k = 0; k < n; k++) for (int j = 0; j < n; j++)
            A2[i][j] += A[i][k] * A[k][j];

    memset(tmp_result, 0, sizeof(uint32_t) * n * n);
    parallel_for(int i = 0; i < n; i++)
        parallel_for(int j = 0; j < n; j++)
            tmp_result[i * n + j] = A[i][j] * A2[i][j];
    result = ref_get_par_sum(tmp_result, n * n);

    result /= 6; //ij与ji都计算过一次，需要再次除以2

    parallel_for(int i = 0; i < n; i++)
    {
        delete[] A[i];
        delete[] A2[i];
    }
    delete[] A;
    delete[] A2;

    return result;
}