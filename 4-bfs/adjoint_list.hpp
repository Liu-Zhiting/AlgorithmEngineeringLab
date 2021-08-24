#pragma once

typedef struct Node
{
    uint32_t value;
    struct Node *next;
} Node;
typedef Node *Linklist;

class AdjointList
{
private:
    void initialize();
    void dispose();

public:
    uint32_t vertex_count;
    uint32_t edge_count;
    uint32_t *out_degree;
    uint32_t *in_degree;
    Node *vertex;

    AdjointList() : vertex_count(0), edge_count(0), out_degree(nullptr), vertex(nullptr){};
    AdjointList(const int vertex_count) : vertex_count(vertex_count), edge_count(0), out_degree(nullptr), vertex(nullptr) { initialize(); };
    AdjointList(const AdjointList &other);
    ~AdjointList() { dispose(); };

    bool operator==(const AdjointList &other) const;
    AdjointList &operator=(const AdjointList &other);

    uint32_t get_vertex_count() const { return this->vertex_count; };
    uint32_t get_edge_count() const { return this->edge_count; };
    bool add_edge(const int from, const int to);
    bool search_edge(const int from, const int to) const;
    bool remove_edge(const int from, const int to);

    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    void save_file_text(const char *filename) const;
    void save_file_binary(const char *filename) const;

    uint32_t **convert_to_adjoint_matrix() const;
    void dump_adjoint_list() const;
};