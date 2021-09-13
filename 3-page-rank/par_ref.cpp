#include "utils.hpp"
#include "static_adjoint_list.hpp"
#include "solution.hpp"
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#define parallel_for cilk_for
#define parallel_spawn cilk_spawn
#define parallel_sync cilk_sync

double ref_get_par_sum(const double *const array, const unsigned long length)
{
    double result = 0.0;
    if (length <= 256)
    {
        for (long i = 0; i < length; i++)
            result += array[i];
        return result;
    }
    double x, y;
    parallel_spawn x = ref_get_par_sum(array, length / 2);
    y = ref_get_par_sum((array + length / 2), length - length / 2);
    parallel_sync;
    result = x + y;
    return result;
}

void par_ref(const Graph &graph, Solution &solution)
{
    //init
    const double THRESHOLD = 1e-7;
    const double d = 0.85;
    const int n = graph.get_vertex_count();
    double *delta = new double[n];
    double max_delta = 0.0, min_delta = INFINITY, avg_delta = INFINITY;
    int counter = 0;
    for (int i = 0; i < n; i++)
        solution.value[i] = 1.0 / n;

    // iterate to calculate page rank
    while (avg_delta > THRESHOLD)
    {
        parallel_for (int i = 0; i < n; i++)
        {
            double s = 0.0;
            double tmp = 0.0;            
            for (int j = 0; j < graph.out_degree[i]; j++)
                s += solution.value[graph.neighbor[i][j]] / graph.out_degree[graph.neighbor[i][j]];
            tmp = (1 - d) / n + d * s;
            delta[i] = abs(tmp - solution.value[i]);
            solution.value[i] = tmp;
        };

        avg_delta = (ref_get_par_sum(delta, n) / n);
        
        counter++;
    }

    delete[] delta;
}