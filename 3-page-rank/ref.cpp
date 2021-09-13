#include "utils.hpp"
#include "static_adjoint_list.hpp"
#include "solution.hpp"

double ref_get_sum(const double *const array, const unsigned long length)
{
    double result = 0.0;
    for (long i = 0; i < length; i++)
        result += array[i];
    return result;
}

void ref(const Graph &graph, Solution &solution)
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
        for (int i = 0; i < n; i++)
        {
            double s = 0.0;
            double tmp = 0.0;            
            for (int j = 0; j < graph.out_degree[i]; j++)
                s += solution.value[graph.neighbor[i][j]] / graph.out_degree[graph.neighbor[i][j]];
            tmp = (1 - d) / n + d * s;
            delta[i] = abs(tmp - solution.value[i]);
            solution.value[i] = tmp;
        };

        avg_delta = (ref_get_sum(delta, n) / n);
        
        counter++;
    }

    delete[] delta;
}