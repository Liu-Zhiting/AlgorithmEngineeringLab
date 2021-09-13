#pragma once

class StaticAdjointList
{
private:
    void dispose();
    uint32_t vertex_count;
    uint32_t edge_count;

public:
    uint32_t *out_degree;
    uint32_t *in_degree;
    uint32_t **neighbor;

    StaticAdjointList() : vertex_count(0),
                          edge_count(0),
                          out_degree(nullptr),
                          in_degree(nullptr),
                          neighbor(nullptr){};
    StaticAdjointList(const StaticAdjointList &other);
    StaticAdjointList(const char *filename, bool is_binary);
    ~StaticAdjointList() { dispose(); };

    bool operator==(const StaticAdjointList &other) const;
    StaticAdjointList &operator=(const StaticAdjointList &other);

    uint32_t get_vertex_count() const { return this->vertex_count; };
    uint32_t get_edge_count() const { return this->edge_count; };
    bool search_edge(const int from, const int to) const;

    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    void save_file_text(const char *filename) const;
    void save_file_binary(const char *filename) const;

    uint32_t **convert_to_adjoint_matrix() const;
    void dump() const;
};