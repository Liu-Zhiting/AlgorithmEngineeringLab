#pragma once
#include "utils.hpp"

class AdjointList
{
private:
    void initialize();
    void dispose();

public:
    uint32_t vertex_count;
    uint32_t *out_degree;
    Node *vertex;

    AdjointList() : vertex_count(0), out_degree(nullptr), vertex(nullptr){};
    AdjointList(int vertex_count) : vertex_count(vertex_count) { initialize(); };
    AdjointList(const AdjointList &other);
    AdjointList(const char *binary_filename);
    ~AdjointList() { dispose(); };

    bool operator==(const AdjointList &other) const;
    AdjointList &operator=(const AdjointList &other);

    uint32_t get_vertex_count() { return this->vertex_count; };
    void add_edge(int from, int to);
    bool search_edge(int from, int to) const;
    uint32_t** convert_to_adjoint_matrix() const;

    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    void save_file_binary(const char *binary_filename);
    void dump_adjoint_list();
};