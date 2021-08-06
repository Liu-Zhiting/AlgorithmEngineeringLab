#include "utils.hpp"

string mode;
int file_type_id, mode_id;

ostringstream ostr;

void generate_random(AdjointList &graph);

void (*generate_func[2])(AdjointList &) = {
    generate_random};

int main(int argc, char **argv)
{
    AdjointList graph;
    if (argc < 4)
    {
        cerr << "Usage: <file_type:text|binary> <mode:random|sparse> <vertex_count>" << endl;
        return 1;
    }
    try
    {
        if (0 == strcmp(argv[1], "text"))
            file_type_id = 0;
        else if (0 == strcmp(argv[1], "binary"))
            file_type_id = 1;
        else
            throw argv[1];
        if (0 == strcmp(argv[2], "random"))
            mode_id = 0;
        else
            throw argv[2];
        mode = argv[2];
        int vertex_vount = atoi(argv[3]);
        AdjointList g(vertex_vount);
        graph = g;
    }
    catch (char *argument)
    {
        cerr << "Argument \'" << argument << "\' invalid" << endl;
        return 1;
    }
    catch (exception e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    ostr << "adjoint_list_" << mode << '_' << graph.vertex_count;

    generate_func[mode_id](graph);
    if (0 == file_type_id)
    {
        ostr << ".txt";
        graph.save_file_text(ostr.str().c_str());
    }
    else
    {
        ostr << ".bin";
        graph.save_file_binary(ostr.str().c_str());
    }

    return 0;
}

void generate_random(AdjointList &graph)
{
    int n = graph.vertex_count;
    int iter_rounds = (n) * (n - 1) / 2;
    iter_rounds = (int)(((rand() % 100) / 100.0) * iter_rounds);
    uint32_t u, v;
    for (int i = 0; i < iter_rounds; i++)
    {
        u = rand() % n;
        v = rand() % n;
        while (u == v)
            v = rand() % n;
        if (graph.search_edge(u, v) || graph.search_edge(v, u))
            continue;
        graph.add_edge(u, v);
        graph.add_edge(v, u);
    }
}