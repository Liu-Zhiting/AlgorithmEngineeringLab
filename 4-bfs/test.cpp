#include "utils.hpp"
#include "test.hpp"
#include "static_adjoint_list.hpp"
#include "solution.hpp"

StaticAdjointList Source;
Solution TheSolution;
string FileName = "";
string TimingResult = "";

vector<TestCase> test_list =
{
    {"top_down",top_down},
    {"buttom_up",buttom_up},
    {"hybrid",hybrid}
};

bool parse_args(int argc, char **argv)
{
    if (argc < 2)
        return false;
    FileName = argv[1];
    return true;
}

bool initialize()
{
    bool load_result = Source.load_data_binary(FileName.c_str());
    if (!load_result)
        return false;
    TheSolution.attach_to_graph(Source);
    return true;
}

void print_data_info()
{
    cout << "Data info:" << endl
         << "Vertex Count: " << Source.get_vertex_count() << endl
         << "Edge Count: " << Source.get_edge_count() << endl;
}

TestResult run_and_measure_time(TestCase test_case)
{
    TestResult test_result;
    test_result.name = test_case.name;
    test_result.nworkers = __cilkrts_get_nworkers();
    try
    {
        TheSolution.clear();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        test_case.func(Source, TheSolution);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        test_result.time = time_span.count();
        return test_result;
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred while running " << test_result.name << endl;
        cerr << e.what() << endl;
        test_result.time = NAN;
        return test_result;
    }
}

vector<TestResult> test_all()
{
    vector<TestResult> results;
    TestResult current_result;

    TestCase ref_case = {"ref",ref};
    TestResult ref_result = run_and_measure_time(ref_case);
    Solution compare(TheSolution);
    ref_result.correctness = true;
    results.push_back(ref_result);

    for (int i = 0; i < test_list.size(); i++)
    {
        current_result = run_and_measure_time(test_list[i]);
        current_result.correctness = (TheSolution == compare);
        results.push_back(current_result);
    }

    return results;
}

void dump_result(vector<TestResult> &cases)
{
    stringstream ss;

    // init tab width
    vector<int> tab_width(cases.size()+1);
    int tail = 0;
    tab_width[0] = 12;   // magic number, length of "Worker(s)"
    for(int i = 0; i < cases.size(); i++)
        tab_width[i+1] = max((int)cases[i].name.length() + 2,10);
    ss << left;

    // dump headline
    if (1 == cases[0].nworkers)
    {
        ss << setw(tab_width[tail++]) << "Worker(s)";
        for (int i = 0; i < cases.size(); i++)
            ss << setw(tab_width[tail++]) << cases[i].name;
        ss << endl;
        tail = 0;
    }

    ss << setw(tab_width[tail++]) << __cilkrts_get_nworkers();
    for (int i = 0; i < cases.size(); i++)
    {
        ss << setw(tab_width[tail++]);
        if (!cases[i].correctness)
            ss << "Wrong Ans";
        else if (NAN == cases[i].time)
            ss << "#Error";
        else
            ss << cases[i].time;
    }
    ss << endl;

    TimingResult += ss.str();
}