#include "utils.hpp"
#include "adjoint_list.hpp"
#include "solution.hpp"
#include <queue>

void ref_BFS(const AdjointList &graph, Solution &solution)
{
    queue<int> q;
    vector<bool> visited(graph.vertex_count);
    parallel_for(int i = 0; i < graph.vertex_count; i++)
        visited[i] = false;

    solution.distance[ROOT_ID] = 0;
    visited[ROOT_ID] = true;
    q.push(ROOT_ID);
    while (!q.empty())
    {
        for (Node *p = graph.vertex[q.front()].next; p != nullptr; p = p->next)
        {
            if (visited[p->value])
                continue;
            q.push(p->value);
            visited[p->value] = true;
            solution.distance[p->value] = solution.distance[q.front()] + 1;
        }
        q.pop();
    }
}