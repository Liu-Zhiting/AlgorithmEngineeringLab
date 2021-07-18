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
public:
    uint32_t vertex_count;
    uint32_t *out_degree;
    Node *vertex;

    AdjointList() : vertex_count(0), out_degree(nullptr), vertex(nullptr){};
    AdjointList(int vertex_count) : vertex_count(vertex_count) { initialize(); };
    AdjointList(const AdjointList &other);
    ~AdjointList() { dispose(); };

    void initialize();
    void dispose();

    bool operator==(const AdjointList &other) const;
    AdjointList &operator=(const AdjointList &other);

    uint32_t get_vertex_count() { return this->vertex_count; };
    void add_edge(int from, int to);
    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    void save_file_text(const char *filename) const;
    void save_file_binary(const char *filename) const;
    void dump_adjoint_list() const;
};