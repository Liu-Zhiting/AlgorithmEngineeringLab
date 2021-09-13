#include "utils.hpp"

class Solution
{
private:
    void initialize();
    void dispose();

public:
    uint32_t size;
    double *value;
    Solution() : size(0), value(nullptr){};
    Solution(uint32_t size) : value(nullptr) { initialize(); };
    Solution(const Solution &other);
    ~Solution() { dispose(); };

    void attach_to_graph(const Graph &graph);
    void clear() { memset(value, 0, size * sizeof(double)); };
    void dump() const;

    bool operator==(const Solution &other) const;
    Solution &operator=(const Solution &other);
};