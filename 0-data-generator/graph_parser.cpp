#include "utils.hpp"
#include "adjoint_list.hpp"

using ui = unsigned int;

string mode, filename;

map<string, void (*)(const char *)> func_map;

void init_func_map();
void parseRawSNAPText(const char *);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "Usage: graph_parser <mode:SNAP|DIMACS|DIMACS-clq> <filename>" << endl;
        exit(1);
    }
    mode = argv[1];
    filename = argv[2];

    init_func_map();

    func_map.find(mode)->second(filename.c_str());
}

void init_func_map()
{
    func_map.insert(make_pair("SNAP", parseRawSNAPText));
}

void parseRawSNAPText(const char *filepath)
{
    std::ifstream infile;
    char buf[1024];
    std::vector<std::pair<ui, ui>> epairs;
    std::vector<ui> nodes;
    //FILE *f = Utility::open_file(filepath, "r");
    infile.open(filepath, std::ios::in);
    if (!infile.is_open())
    {
        fprintf(stderr, "can not find file %s\n", filepath);
        exit(1);
    }
    int max_id = 0;
    int from, to;
    while (infile.getline(buf, 1024))
    {
        char *p = buf;
        while (*p == ' ' && *p != '\0')
            p++;
        if (*p == '#' || *p == '\0')
            continue;
        std::stringstream ss(buf);
        ss >> from >> to;
        //去除自环边
        if (from != to)
        {
            epairs.push_back(std::make_pair(from, to));
            epairs.push_back(std::make_pair(to, from));
            nodes.push_back(from);
            nodes.push_back(to);
        }
    }
    infile.close();

    //去除SNAP中的重点
    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    //去除SNAP中的重边
    sort(epairs.begin(), epairs.end());
    epairs.erase(unique(epairs.begin(), epairs.end()), epairs.end());

    //去除孤立点。对剩下的点重新编号。 in p2p-Gnutella04, 10452 is a isolate vertex
    ui contn = 1;
    std::map<ui, ui> idmp;
    for (ui i = 0; i < nodes.size(); i++)
    {
        idmp[nodes[i]] = i;
        if (nodes[i] != i)
        {
            contn = 0;
        }
    }
    if (contn == 0)
        printf("Node ids are not preserved! \n");

    AdjointList graph(nodes.size());
    for (int i = 0; i < epairs.size(); i++)
    {
        cout << "progress: " << i << " / " << epairs.size() << "                   ";
        graph.add_edge((uint32_t)epairs[i].first, (uint32_t)epairs[i].second);
        cout << '\r';
    }
    cout << "                                   ";
    graph.dump_adjoint_list();
    stringstream ss;
    ss << filename << ".bin";
    cout << ss.str() << endl;
    graph.save_file_binary(ss.str().c_str());
}
