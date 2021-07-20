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

bool AdjointList::load_data_text(const char *binary_filename)
{
    //init
    ifstream fin;
    fin.open(binary_filename);

    //read metadata and initialize
    fin >> vertex_count;
    if (vertex_count <= 0)
    {
        cerr << "vertex_count <= 0" << endl;
        return false;
    }
    initialize();
    for (int i = 0; i < vertex_count; i++)
        fin >> out_degree[i];

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

bool AdjointList::load_data_binary(const char *binary_filename)
{
    //init
    ifstream fin;
    fin.open(binary_filename, ios::binary);

    //read metadata and initialize
    fin.read((char *)&vertex_count, sizeof(uint32_t));
    if (vertex_count <= 0)
    {
        cerr << "vertex_count <= 0" << endl;
        return false;
    }
    initialize();
    fin.read((char *)out_degree, sizeof(uint32_t) * vertex_count);

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

void AdjointList::add_edge(int from, int to)
{
    this->out_degree[from]++;
    Node *p = this->vertex[from].next;
    this->vertex[from].next = new Node();
    this->vertex[from].next->value = to;
    this->vertex[from].next->next = p;
}

void AdjointList::save_file_binary(const char *binary_filename)
{
    //init
    ofstream fout;
    fout.open(binary_filename, ios::binary);

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

void AdjointList::dump_adjoint_list()
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