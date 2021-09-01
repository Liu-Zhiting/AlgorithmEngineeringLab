#include "utils.hpp"

class Solution
{
private:
    void initialize();
    void dispose();

public:
    uint32_t size;
    int32_t *distance;
    Solution() : size(0), distance(nullptr){};
    Solution(uint32_t size) : distance(nullptr) { initialize(); };
    Solution(const Solution &other);
    ~Solution() { dispose(); };

    void attach_to_graph(const Graph &graph);
    void clear() { memset(distance, 0, size * sizeof(int32_t)); };
    void dump() const;

    bool operator==(const Solution &other) const;
    Solution &operator=(const Solution &other);
};