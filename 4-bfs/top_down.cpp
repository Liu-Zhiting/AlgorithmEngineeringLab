#include "utils.hpp"
#include "static_adjoint_list.hpp"
#include "solution.hpp"
#include <queue>

void top_down(const Graph &graph, Solution &solution)
{
    queue<uint32_t> q;
    uint32_t n = graph.get_vertex_count();
    vector<bool> visited(n);
    for(int i = 0; i < n; i++)
        visited[i] = false;

    solution.distance[ROOT_ID] = 0;
    visited[ROOT_ID] = true;
    q.push(ROOT_ID);
    while (!q.empty())
    {
        int i = q.front();
        for (int j = 0; j < graph.out_degree[i]; j++)
        {
            uint32_t v = graph.neighbor[i][j];
            if (visited[v])
                continue;
            q.push(v);
            visited[v] = true;
            solution.distance[v] = solution.distance[q.front()] + 1;
        }
        q.pop();
    }
}