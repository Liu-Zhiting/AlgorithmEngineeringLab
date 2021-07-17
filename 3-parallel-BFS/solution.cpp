#include "utils.hpp"

void Solution::initialize()
{
    if (distance != nullptr)
        return;
    if (size <= 0)
        return;
    distance = new uint32_t[size];
    memset(distance, 0, size * sizeof(uint32_t));
}

void Solution::dispose()
{
    if (nullptr == distance)
        return;
    delete[] distance;
    distance = nullptr;
}

Solution::Solution(const Solution &other) : size(other.size)
{
    if (this == &other)
        return;
    initialize();
    memcpy(distance, other.distance, other.size * sizeof(uint32_t));
}

void Solution::attach_to_graph(const Graph &graph)
{
    dispose();
    size = graph.vertex_count;
    initialize();
}

bool Solution::operator==(const Solution &other) const
{
    if (size != other.size)
        return false;
    return (0 == memcmp(distance, other.distance, size * sizeof(uint32_t)));
}

Solution &Solution::operator=(const Solution &other)
{
    if (this == &other)
        return *this;
    dispose();
    size = other.size;
    initialize();
    memcpy(distance, other.distance, size * sizeof(uint32_t));
    return *this;
}