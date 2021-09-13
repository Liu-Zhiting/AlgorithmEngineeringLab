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
    {"par_ref",par_ref},
    {"iterate_rank", iterate_rank},
    {"accurate_rank", accurate_rank}
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

    TestCase ref_case = {"ref", ref};
    cout << "output of ref:" << endl;
    TestResult ref_result = run_and_measure_time(ref_case);
    Solution compare(TheSolution);
    ref_result.correctness = true;
    results.push_back(ref_result);

    for (int i = 0; i < test_list.size(); i++)
    {
        cout << endl << "output of " << test_list[i].name << ':' << endl;
        current_result = run_and_measure_time(test_list[i]);
        current_result.correctness = (TheSolution == compare);
        results.push_back(current_result);
    }

    return results;
}

void dump_result(vector<TestResult> &cases)
{
    // init string stream
    stringstream ss;
    ss << left; // 左对齐
    ss.setf(ios::fixed);
    ss << setprecision(3); // 保留小数点后3位

    // init tab width
    int tail = 0;
    vector<int> tab_width(4);
    tab_width[tail] = 0;
    for (int i = 0; i < cases.size(); i++)
        if (tab_width[tail] < cases[i].name.length())
            tab_width[tail] = cases[i].name.length();
    tab_width[tail++] += 2;   // Name
    tab_width[tail++] = 14; // Correctness
    tab_width[tail++] = 10; // Time(s)
    tab_width[tail++] = 11; // Speed Up

    // dump headline
    tail = 0;
    ss << setw(tab_width[tail++]) << "Name";
    ss << setw(tab_width[tail++]) << "Correctness";
    ss << setw(tab_width[tail++]) << "Time(s)";
    ss << setw(tab_width[tail++]) << "Speed Up" << endl;

    // dump data of every cases
    for (int i = 0; i < cases.size(); i++)
    {
        tail = 0;
        ss << setw(tab_width[tail++]) << cases[i].name;
        ss << setw(tab_width[tail++]) << (cases[i].correctness ? "True" : "False");
        if (NAN == cases[i].time)
        {
            ss << setw(tab_width[tail++]) << "#Error";
            ss << setw(tab_width[tail++]) << "#Error";
        }
        else
        {
            ss << setw(tab_width[tail++]) << cases[i].time;
            ss << setw(tab_width[tail++]) << (cases[0].time / cases[i].time);
        }
        ss << endl;
    }

    // flush to output string
    TimingResult += ss.str();
    return;
}