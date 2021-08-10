#include "utils.hpp"
#include "adjoint_list.hpp"
#include "solution.hpp"

double get_sum(const double *const array, const unsigned long length)
{
    double result = 0.0;
    if (length <= 256)
    {
        for (long i = 0; i < length; i++)
            result += array[i];
        return result;
    }
    double x, y;
    parallel_spawn x = get_sum(array, length / 2);
    y = get_sum((array + length / 2), length - length / 2);
    parallel_sync;
    result = x + y;
    return result;
}

void ref(const AdjointList &graph, Solution &solution)
{
    //init
    const double THRESHOLD = 1e-7;
    const double d = 0.85;
    double *delta = new double[graph.vertex_count];
    double max_delta = 0.0, min_delta = INFINITY, avg_delta = INFINITY;
    int counter = 0;
    AdjointList graph_inv(graph.vertex_count);
    for (int i = 0; i < graph.vertex_count; i++)
        solution.value[i] = 1.0 / graph.vertex_count;

    // convert graph to array-adjlist to enhance performance
    uint32_t **adj_list = new uint32_t *[graph.vertex_count];
    for (int i = 0; i < graph.vertex_count; i++)
    {
        adj_list[i] = new uint32_t[graph.out_degree[i]];
    };
    parallel_for(int i = 0; i < graph.vertex_count; i++)
    {
        int j = 0;
        for (Node *p = graph.vertex[i].next; p != nullptr; p = p->next)
        {
            adj_list[i][j] = p->value;
            j++;
        }
    };

    // iterate to calculate page rank
    while (avg_delta > THRESHOLD)
    {
        parallel_for (int i = 0; i < graph.vertex_count; i++)
        {
            double s = 0.0;
            double tmp = 0.0;            
            for (int j = 0; j < graph.out_degree[i]; j++)
                s += solution.value[adj_list[i][j]] / graph.out_degree[adj_list[i][j]];
            tmp = (1 - d) / graph.vertex_count + d * s;
            delta[i] = abs(tmp - solution.value[i]);
            solution.value[i] = tmp;
        };

        avg_delta = (get_sum(delta, graph.vertex_count) / graph.vertex_count);
        
        counter++;
    }

    delete[] delta;
    for (int i = 0; i < graph.vertex_count; i++)
        delete[] adj_list[i];
    delete[] adj_list;
}