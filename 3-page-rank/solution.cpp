#include "utils.hpp"
#include "adjoint_list.hpp"
#include "solution.hpp"

void Solution::initialize()
{
    if (value != nullptr)
        return;
    if (size <= 0)
        return;
    value = new double[size];
    memset(value, 0, size * sizeof(double));
}

void Solution::dispose()
{
    if (nullptr == value)
        return;
    delete[] value;
    value = nullptr;
}

Solution::Solution(const Solution &other) : size(other.size), value(nullptr)
{
    if (this == &other)
        return;
    initialize();
    memcpy(value, other.value, other.size * sizeof(double));
}

void Solution::attach_to_graph(const AdjointList &graph)
{
    dispose();
    size = graph.vertex_count;
    initialize();
}

bool Solution::operator==(const Solution &other) const
{
    if (size != other.size)
        return false;
    const double _EPSILON = 1e-5;
    for(int i = 0; i < size; i++)
        if(abs(value[i] - other.value[i])>_EPSILON)
            return false;
    return true;
}

Solution &Solution::operator=(const Solution &other)
{
    if (this == &other)
        return *this;
    dispose();
    size = other.size;
    initialize();
    memcpy(value, other.value, size * sizeof(double));
    return *this;
}

void Solution::dump() const
{
    for (int i = 0; i < size; i++)
        cout << "value[" << i << "] = " << value[i] << endl;
}