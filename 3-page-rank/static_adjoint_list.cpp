#include "utils.hpp"
#include "static_adjoint_list.hpp"
#include "adjoint_list.hpp"

StaticAdjointList::StaticAdjointList(const AdjointList &other)
{
    // init count and degree
    vertex_count = other.vertex_count;
    edge_count = other.edge_count;
    out_degree = new uint32_t[vertex_count];
    memcpy(out_degree, other.out_degree, vertex_count*sizeof(uint32_t));
    
    // init vertex
    vertex = new uint32_t*[vertex_count];
    for(int i = 0; i < vertex_count; i++)
        vertex[i] = new uint32_t[out_degree[i]];
    for(int i = 0; i < vertex_count; i++)
    {
        int j = 0;
        for(Node*p = other.vertex[i].next;p != nullptr;p = p->next)
        {
            vertex[i][j] = p->value;
            j++;
        }
    }
}

StaticAdjointList::StaticAdjointList(const char* filename, bool is_binary)
{
    if(is_binary)
        load_data_binary(filename);
    else
        load_data_text(filename);
}

void StaticAdjointList::dispose()
{
    if (nullptr == vertex)
        return;
    for (int i = 0; i < vertex_count; i++)
        delete [] vertex[i];
    delete[] vertex;
    vertex = nullptr;
    if (nullptr == out_degree)
        return;
    delete[] out_degree;
    out_degree = nullptr;
}

bool StaticAdjointList::load_data_text(const char *filename)
{
    // init
    ifstream fin;
    fin.open(filename);
    if (!fin.good())
        return false;

    // read vertex count
    fin >> vertex_count;
    if (vertex_count <= 0)
    {
        cerr << "vertex_count <= 0" << endl;
        return false;
    }

    // read degree
    out_degree = new uint32_t[vertex_count];
    in_degree = new uint32_t[vertex_count];
    memset(in_degree, 0, vertex_count * sizeof(uint32_t));
    for (int i = 0; i < vertex_count; i++)
    {
        fin >> out_degree[i];
        edge_count += out_degree[i];
    }

    // init vertex
    vertex = new uint32_t*[vertex_count];
    for(int i = 0; i < vertex_count; i++)
        vertex[i] = new uint32_t[out_degree[i]];

    // read data
    for (int i = 0; i < vertex_count; i++)
    {
        for(int j = 0;j < out_degree[i];j++)
        {
            fin >> vertex[i][j];
            in_degree[vertex[i][j]]++; // TODO potential memory leak
        }
    }

    // end read
    fin.close();
    return true;
}

bool StaticAdjointList::load_data_binary(const char *filename)
{
    //init
    ifstream fin;
    fin.open(filename, ios::binary);
    if (!fin.good())
        return false;

    //read vertex_count
    fin.read((char *)&vertex_count, sizeof(uint32_t));
    if (vertex_count <= 0)
    {
        cerr << "vertex_count <= 0" << endl;
        return false;
    }

    // read degree
    out_degree = new uint32_t[vertex_count];
    in_degree = new uint32_t[vertex_count];
    memset(in_degree, 0, vertex_count * sizeof(uint32_t));
    fin.read((char *)out_degree, sizeof(uint32_t) * vertex_count);
    for(int i = 0; i < vertex_count; i++)
        edge_count += out_degree[i];

    // init vertex[]
    vertex = new uint32_t*[vertex_count];
    for(int i = 0; i < vertex_count; i++)
        vertex[i] = new uint32_t[out_degree[i]];

    //read data
    for (int i = 0; i < vertex_count; i++)
        fin.read((char*)vertex[i],out_degree[i]*sizeof(uint32_t));

    //end read
    fin.close();

    // set in_degree
    for(int i = 0; i < vertex_count; i++)
        for(int j = 0; j < out_degree[i]; j++)
            in_degree[vertex[i][j]]++;  // TODO potential memory leak

    return true;
}

bool StaticAdjointList::operator==(const StaticAdjointList &other) const
{
    if (this == &other)
        return true;
    if (vertex_count != other.vertex_count)
        return false;
    if (nullptr == out_degree && nullptr == other.out_degree)
        return true;
    if ((nullptr != out_degree && nullptr == other.out_degree) || (nullptr == out_degree && nullptr != other.out_degree))
        return false;
    if (nullptr == in_degree && nullptr == other.in_degree)
        return true;
    if ((nullptr != in_degree && nullptr == other.in_degree) || (nullptr == in_degree && nullptr != other.in_degree))
        return false;
    for (int i = 0; i < vertex_count; i++)
    {
        if (out_degree[i] != other.out_degree[i])
            return false;
        if (in_degree[i] != other.in_degree[i])
            return false;
    }
    if (nullptr == vertex && nullptr == other.vertex)
        return true;
    if ((nullptr != vertex && nullptr == other.vertex) || (nullptr == vertex && nullptr != other.vertex))
        return false;
    for (int i = 0; i < vertex_count; i++)
    {
        if(memcmp(vertex[i],other.vertex[i],out_degree[i] * sizeof(uint32_t)))
            return false;
    }
    return true;
}

StaticAdjointList &StaticAdjointList::operator=(const StaticAdjointList &other)
{
    if (this == &other)
        return *this;
    dispose();

    vertex_count = other.vertex_count;
    out_degree = new uint32_t[vertex_count];
    in_degree = new uint32_t[vertex_count];
    memcpy(out_degree, other.out_degree, vertex_count * sizeof(uint32_t));
    memcpy(in_degree, other.in_degree, vertex_count * sizeof(uint32_t));

    vertex = new uint32_t*[vertex_count];
    for (int i = 0; i < vertex_count; i++)
    {
        vertex[i] = new uint32_t[out_degree[i]];
        memcpy(vertex[i],other.vertex[i],out_degree[i] * sizeof(uint32_t));
    }

    return *this;
}

bool StaticAdjointList::search_edge(const int from, const int to) const
{
    for (int j = 0; j < out_degree[from];j++)
        if (vertex[from][j] == to)
            return true;
    return false;
}  // TODO use binary search

void StaticAdjointList::save_file_text(const char *filename) const
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
        for (int j = 0; j < out_degree[i]; j++)
            fout << vertex[i][j] << ' ';
        fout << endl;
    }

    //close file
    fout.close();
}

void StaticAdjointList::save_file_binary(const char *filename) const
{
    //init
    ofstream fout;
    fout.open(filename, ios::binary);

    //write meta data
    fout.write((char *)&vertex_count, sizeof(uint32_t));
    fout.write((char *)out_degree, sizeof(uint32_t) * vertex_count);

    //write edge data
    for (int i = 0; i < vertex_count; i++)
        fout.write((char *)vertex[i],sizeof(uint32_t) * out_degree[i]);

    //close file
    fout.close();
}

void StaticAdjointList::dump_adjoint_list() const
{
    cout << "vertex_count: " << vertex_count << endl;
    cout << "edge_count" << edge_count << endl;
    for (int i = 0; i < vertex_count; i++)
    {
        cout << "vertex " << i;
        for (int j = 0;j < out_degree[i]; j++)
            cout << " -> " << vertex[i][j];
        cout << endl;
    }
}

uint32_t **StaticAdjointList::convert_to_adjoint_matrix() const
{
    uint32_t **result = new uint32_t *[vertex_count];
    for(int i = 0; i < vertex_count; i++)
    {
        result[i] = new uint32_t[vertex_count];
        memset(result[i], 0, vertex_count * sizeof(uint32_t));
    }
    for(int i = 0; i < vertex_count; i++)
        for (int j = 0;j < out_degree[i]; j++)
            result[i][j] = 1;
    
    return result;
}