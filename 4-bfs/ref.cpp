#include "utils.hpp"
#include "static_adjoint_list.hpp"
#include "solution.hpp"
#include "bitmap.hpp"
#include <vector>
#include <algorithm>

int ref_counter;
bool* visited = nullptr;

// mu: the number of edges to check from unexplored vertices
int ref_get_mu(const Graph &graph, const Solution &solution)
{
    int mu = 0;
    for (int i = 0; i < graph.get_vertex_count(); i++)
        if (-1 == solution.distance[i])
            mu += graph.out_degree[i];
    return mu;
}

// mf: the number of edges to check from the frontier
int ref_get_mf(const Graph &graph, const vector<uint32_t> &frontier)
{
    int mf = 0;
    for (int i = 0; i < frontier.size(); i++)
        mf += graph.out_degree[frontier[i]];
    return mf;
}

void ref_vector2bitmap(
    const vector<uint32_t> *const frontier,
    Bitmap &frontier_bitmap)
{
    frontier_bitmap.clear();
    for (int i = 0; i < frontier->size(); i++)
    {
        frontier_bitmap.set(frontier->at(i), true);
    }
}

void ref_bitmap2vector(
    const Bitmap &frontier_bitmap,
    vector<uint32_t> *const frontier)
{
    for (int i = 0; i < frontier_bitmap.get_capacity(); i++)
    {
        if (frontier_bitmap.at(i))
            frontier->push_back(i);
    }
}

void ref_bottom_up_step(
    const Graph &graph,
    Solution &solution,
    const Bitmap &frontier_bitmap,
    Bitmap &next_bitmap)
{
    for (int i = 0; i < graph.get_vertex_count(); i++)
    {
        if (visited[i])
            continue;
        for (int j = 0; j < graph.out_degree[i]; j++)
        {
            if (frontier_bitmap.at(graph.neighbor[i][j]))
            {
                solution.distance[i] = ref_counter;
                visited[i] = true;
                next_bitmap.set(i, true);
                break;
            }
        }
    }
    ref_counter++;
}

void ref_top_down_step(
    const Graph &graph,
    Solution &solution,
    const vector<uint32_t> *const frontier,
    vector<uint32_t> *const next)
{
    for (int i = 0; i < frontier->size(); i++)
    {
        uint32_t u = (*frontier)[i];
        for (int j = 0; j < graph.out_degree[u]; j++)
        {
            uint32_t v = graph.neighbor[u][j];
            if (visited[v])
                continue;
            visited[v] = true;
            solution.distance[v] = ref_counter;
            next->push_back(v); // write race here
        }
    }
    ref_counter++;
}

template <typename T>
void ref_swap_pointer(T **a, T **b)
{
    T *tmp = *a;
    *a = *b;
    *b = tmp;
}

void ref(const Graph &graph, Solution &solution)
{
    ref_counter = 1;
    uint32_t n = graph.get_vertex_count();
    bool is_top_down = true;
    visited = new bool[n];
    memset(visited,0,sizeof(bool)*n);

    // init meta data
    const int ALPHA = 14;
    const int BETA = 24;

    // init frontier and next
    vector<uint32_t> q1, q2;
    vector<uint32_t> *frontier = &q1, *next = &q2;
    Bitmap b1(n), b2(n);
    Bitmap *frontier_bitmap = &b1, *next_bitmap = &b2;

    // push ROOT
    frontier->push_back(ROOT_ID);
    solution.distance[ROOT_ID] = 0;
    visited[0] = true;

    while ((is_top_down && !frontier->empty()) 
        || (!is_top_down && !frontier_bitmap->is_empty()))
    {
        if (is_top_down)
        {
            ref_top_down_step(graph, solution, frontier, next);
            ref_swap_pointer(&frontier, &next);
            next->clear();
            if (ref_get_mf(graph, *frontier) > ref_get_mu(graph, solution) / ALPHA)
            {
                ref_vector2bitmap(frontier, *frontier_bitmap);
                next_bitmap->clear();
                is_top_down = false;
            }
        }
        else
        {
            ref_bottom_up_step(graph, solution, *frontier_bitmap, *next_bitmap);
            ref_swap_pointer(&frontier_bitmap, &next_bitmap);
            next_bitmap->clear();
            if (frontier_bitmap->get_num_of_1() < n / BETA)
            {
                ref_bitmap2vector(*frontier_bitmap, frontier);
                next->clear();
                is_top_down = true;
            }
        }
    }

    delete[] visited;
}