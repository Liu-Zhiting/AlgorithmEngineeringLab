#include "utils.hpp"
#include "adjoint_list.hpp"

using ui = unsigned int;
using ULL = unsigned long long;

string mode, filename;

map<string, void (*)(const char *)> func_map;

void init_func_map();
void parseRawSNAPText(const char *);
void parseRawSNAPTextLL(const char *filepath);   // when integer is long long

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "Usage: graph_parser <mode:SNAP|SNAP-LL|DIMACS|DIMACS-clq> <filename>" << endl;
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
    func_map.insert(make_pair("SNAP-LL",parseRawSNAPTextLL));
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
    ui from, to;
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
        // 使用重新编的号码
        from = (uint32_t)epairs[i].first;
        from = idmp.find(from)->second;
        to = (uint32_t)epairs[i].second;
        to = idmp.find(to)->second;

        cout << "progress: " << i << " / " << epairs.size() << "                   ";
        graph.add_edge(from, to);
        cout << '\r';
    }
    cout << "                                   ";
    stringstream ss;
    ss << filename << ".bin";
    cout << ss.str() << endl;
    graph.save_file_binary(ss.str().c_str());
}

void parseRawSNAPTextLL(const char *filepath)
{
    //init
    std::ifstream infile;
    char buf[1024];
    std::map<string, uint32_t> idmp;
    std::vector<std::pair<uint32_t, uint32_t>> epairs;
    std::vector<uint32_t> nodes;
    //FILE *f = Utility::open_file(filepath, "r");
    infile.open(filepath, std::ios::in);
    if (!infile.is_open())
    {
        fprintf(stderr, "can not find file %s\n", filepath);
        exit(1);
    }
    int max_id = 0;
    string from, to;
    ui edge_counter = 1;
    uint32_t idmp_counter = 0;
    while (infile.getline(buf, 1024))
    {
        cout << edge_counter << '\r';
        edge_counter++;
        char *p = buf;
        while (*p == ' ' && *p != '\0')
            p++;
        if (*p == '#' || *p == '\0')
            continue;
        std::stringstream ss(buf);
        ss >> from;
        ss >> to;
        //去除自环边
        if (from != to)
        {
            cout << "Edge detected: " << edge_counter << '\r';
            if(idmp.end() == idmp.find(from))
            {
                idmp.insert(make_pair(from,idmp_counter));
                idmp_counter++;
            }
            if(idmp.end() == idmp.find(to))
            {
                idmp.insert(make_pair(to,idmp_counter));
                idmp_counter++;
            }
            epairs.push_back(make_pair(idmp.find(from)->second, idmp.find(to)->second));
            epairs.push_back(make_pair(idmp.find(to)->second, idmp.find(from)->second));
            nodes.push_back(idmp.find(from)->second);
            nodes.push_back(idmp.find(to)->second);
        }
    }
    infile.close();

    //去除SNAP中的重点
    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    //去除SNAP中的重边
    sort(epairs.begin(), epairs.end());
    epairs.erase(unique(epairs.begin(), epairs.end()), epairs.end());

    // //去除孤立点。对剩下的点重新编号。 in p2p-Gnutella04, 10452 is a isolate vertex
    // ui contn = 1;
    
    // for (ULL i = 0; i < nodes.size(); i++)
    // {
    //     idmp[nodes[i]] = i;
    // }
    // printf("Node ids are not preserved! \n");

    AdjointList graph(nodes.size());
    uint32_t from_id,to_id;
    
    for (int i = 0; i < epairs.size(); i++)
    {
        // 使用重新编的号码
        // from = epairs[i].first;
        from_id = epairs[i].first;
        // to = epairs[i].second;
        to_id = epairs[i].second;

        cout << "progress: " << i << " / " << epairs.size() << '\r';
        graph.add_edge(from_id, to_id);
    }
    cout << endl << "Convert to adjoint list successfully!" << endl;
    stringstream ss;
    ss << filename << ".bin";
    cout << ss.str() << endl;
    cout << nodes.size() << epairs.size() << endl;
    graph.save_file_binary(ss.str().c_str());
}
