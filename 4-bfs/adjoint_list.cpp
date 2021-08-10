#include "utils.hpp"
#include "adjoint_list.hpp"

void AdjointList::initialize()
{
    if (nullptr != out_degree || nullptr != vertex)
        return;
    if (vertex_count <= 0)
        return;

    this->vertex = new Node[vertex_count];
    this->out_degree = new uint32_t[vertex_count];
    for (int i = 0; i < vertex_count; i++)
    {
        vertex[i].next = nullptr;
        out_degree[i] = 0;
    }
}

void AdjointList::dispose()
{
    if (nullptr == vertex)
        return;

    Node *p = nullptr, *q = nullptr;
    for (int i = 0; i < vertex_count; i++)
    {
        p = vertex[i].next;
        while (p != nullptr)
        {
            q = p->next;
            delete p;
            p = q;
        }
    }
    delete[] vertex;
    if (nullptr == out_degree)
        return;
    delete[] out_degree;
    out_degree = nullptr;
}

bool AdjointList::load_data_text(const char *filename)
{
    //init
    ifstream fin;
    fin.open(filename);
    if (!fin.good())
        return false;

    //read metadata and initialize
    fin >> vertex_count;
    if (vertex_count <= 0)
    {
        cerr << "vertex_count <= 0" << endl;
        return false;
    }
    initialize();
    for (int i = 0; i < vertex_count; i++)
    {
        fin >> out_degree[i];
        edge_count += out_degree[i];
    }

    //read data
    Node *p = nullptr;
    for (int i = 0; i < vertex_count; i++)
    {
        p = &vertex[i];
        for (int j = 0; j < out_degree[i]; j++)
        {
            p->next = new Node();
            p = p->next;
            fin >> (p->value);
            p->next = nullptr;
        }
    }
    p = nullptr;

    //end read
    fin.close();
    return true;
}

bool AdjointList::load_data_binary(const char *filename)
{
    //init
    ifstream fin;
    fin.open(filename, ios::binary);
    if (!fin.good())
        return false;

    //read metadata and initialize
    fin.read((char *)&vertex_count, sizeof(uint32_t));
    if (vertex_count <= 0)
    {
        cerr << "vertex_count <= 0" << endl;
        return false;
    }
    initialize();
    fin.read((char *)out_degree, sizeof(uint32_t) * vertex_count);
    for(int i = 0; i < vertex_count; i++)
        edge_count += out_degree[i];

    //read data
    Node *p = nullptr;
    for (int i = 0; i < vertex_count; i++)
    {
        p = &vertex[i];
        for (int j = 0; j < out_degree[i]; j++)
        {
            p->next = new Node();
            p = p->next;
            fin.read((char *)&(p->value), sizeof(int32_t));
            p->next = nullptr;
        }
    }
    p = nullptr;

    //end read
    fin.close();
    return true;
}

bool AdjointList::operator==(const AdjointList &other) const
{
    if (this == &other)
        return true;
    if (vertex_count != other.vertex_count)
        return false;
    if (nullptr == out_degree && nullptr == other.out_degree)
        return true;
    if ((nullptr != out_degree && nullptr == other.out_degree) || (nullptr == out_degree && nullptr != other.out_degree))
        return false;
    for (int i = 0; i < vertex_count; i++)
    {
        if (out_degree[i] != other.out_degree[i])
            return false;
    }
    if (nullptr == vertex && nullptr == other.vertex)
        return true;
    if ((nullptr != vertex && nullptr == other.vertex) || (nullptr == vertex && nullptr != other.vertex))
        return false;
    for (int i = 0; i < vertex_count; i++)
    {
        Node *p = vertex[i].next, *q = other.vertex[i].next;
        while (nullptr != p && nullptr != q)
        {
            if (p->value != q->value)
                return false;
            p = p->next;
            q = q->next;
        }
        if (nullptr != p || nullptr != q)
            return false;
    }
    return true;
}

AdjointList &AdjointList::operator=(const AdjointList &other)
{
    if (this == &other)
        return *this;
    dispose();
    vertex_count = other.vertex_count;
    initialize();
    memcpy(out_degree, other.out_degree, vertex_count * sizeof(uint32_t));

    Node *p = nullptr, *q = nullptr;
    for (int i = 0; i < vertex_count; i++)
    {
        p = &vertex[i];
        q = &other.vertex[i];
        for (int j = 0; j < out_degree[i]; j++)
        {
            p->next = new Node();
            p = p->next;
            p->value = q->value;
            p->next = nullptr;
        }
    }
    p = nullptr;
    q = nullptr;

    return *this;
}

bool AdjointList::add_edge(const int from, const int to)
{
    if (search_edge(from, to))
        return false;
    out_degree[from]++;
    Node *p = vertex[from].next;
    vertex[from].next = new Node();
    vertex[from].next->value = to;
    vertex[from].next->next = p;
    edge_count++;
    return true;
}

bool AdjointList::search_edge(const int from, const int to) const
{
    for (Node *p = vertex[from].next; p != nullptr; p = p->next)
        if (p->value == to)
            return true;
    return false;
}

bool AdjointList::remove_edge(const int from, const int to)
{
    if (!search_edge(from, to))
        return false;
    out_degree[from]--;
    edge_count--;
    for (Node *p = &vertex[from]; p != nullptr; p = p->next)
    {
        if (to == p->next->value)
        {
            Node *q = p->next;
            p->next = q->next;
            delete q;
            break;
        }
    }
    return true;
}

void AdjointList::save_file_text(const char *filename) const
{
    //init
    ofstream fout;
    fout.open(filename);

    //write meta data
    fout << vertex_count << endl;
    for (int i = 0; i < vertex_count; i++)
        fout << out_degree[i] << ' ';
    fout << endl;

    //write edge data
    for (int i = 0; i < vertex_count; i++)
    {
        for (Node *p = vertex[i].next; p != nullptr; p = p->next)
        {
            fout << p->value << ' ';
        }
        fout << endl;
    }

    //close file
    fout.close();
}

void AdjointList::save_file_binary(const char *filename) const
{
    //init
    ofstream fout;
    fout.open(filename, ios::binary);

    //write meta data
    fout.write((char *)&vertex_count, sizeof(int));
    fout.write((char *)out_degree, sizeof(int) * vertex_count);

    //write edge data
    for (int i = 0; i < vertex_count; i++)
    {
        for (Node *p = vertex[i].next; p != nullptr; p = p->next)
        {
            fout.write((char *)&p->value, sizeof(int));
        }
    }

    //close file
    fout.close();
}

void AdjointList::dump_adjoint_list() const
{
    cout << "vertex_count: " << vertex_count << endl;
    for (int i = 0; i < vertex_count; i++)
    {
        cout << "vertex " << i;
        for (Node *p = vertex[i].next; p != nullptr; p = p->next)
        {
            cout << " -> " << p->value;
        }
        cout << endl;
    }
}

uint32_t **AdjointList::convert_to_adjoint_matrix() const
{
    uint32_t **result = new uint32_t *[vertex_count];
    parallel_for(int i = 0; i < vertex_count; i++)
    {
        result[i] = new uint32_t[vertex_count];
        memset(result[i], 0, vertex_count * sizeof(uint32_t));
    }
    parallel_for(int i = 0; i < vertex_count; i++)
    {
        for (Node *p = vertex[i].next; p != nullptr; p = p->next)
        {
            result[i][p->value] = 1;
        }
    }
    return result;
}