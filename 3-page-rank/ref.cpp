#include "utils.hpp"
#include "adjoint_list.hpp"
#include "solution.hpp"
#include <queue>

void ref(const AdjointList &graph, Solution &solution)
{
    //init
    const double THRESHOLD = 1e-6;
    double d = 0.85, tmp = 0.0;
    double max_delta = 0.0, min_delta = INFINITY;
    int counter = 0;
    AdjointList graph_inv(graph.vertex_count);
    parallel_for (int i = 0; i < graph.vertex_count; i++)
        solution.value[i] = 1.0;

    // iterate to calculate page rank
    while (counter < 12 && (max_delta + min_delta) / 2 > THRESHOLD)
    {
        parallel_for (int i = 0; i < graph.vertex_count; i++)
        {
            double sum = 0.0;
            for (Node *p = graph.vertex[i].next; p != nullptr; p = p->next)   // 这里假定了数据为无向图
                sum += solution.value[p->value] / graph.out_degree[p->value];
            tmp = (1 - d) + d * sum;
            if (abs(tmp - solution.value[i]) < min_delta)
                min_delta = tmp - solution.value[i];
            if (abs(tmp - solution.value[i]) > max_delta)
                max_delta = tmp - solution.value[i];
            solution.value[i] = tmp;
        }
        counter++;
    }
}