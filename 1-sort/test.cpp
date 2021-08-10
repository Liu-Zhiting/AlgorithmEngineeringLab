#include "utils.hpp"
#include "test.hpp"
#include "array.hpp"

Array Source, Result;
string FileName = "";
string TimingResult = "";

vector<TestCase> test_list =
    {
        {"merge_sort", merge_sort},
        {"quick_sort", quick_sort}};

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
    Result = Source;
    return true;
}

void print_data_info()
{
    cout << "Data Info: " << endl
         << "Size: " << Source.get_size() << endl;
}

TestResult run_and_measure_time(TestCase test_case)
{
    TestResult tc;
    tc.name = test_case.name;
    tc.nworkers = __cilkrts_get_nworkers();
    try
    {
        Result = Source;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        Result.sort_with_func(test_case.func);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        tc.time = time_span.count();
        return tc;
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred while running " << tc.name << endl;
        cerr << e.what() << endl;
        tc.time = NAN;
        return tc;
    }
}

vector<TestResult> test_all()
{
    vector<TestResult> results;
    TestResult current_result;

    TestCase ref_case = {"ref", ref};
    TestResult ref_result = run_and_measure_time(ref_case);
    Array compare(Result);
    ref_result.correctness = true;
    results.push_back(ref_result);

    for (int i = 0; i < test_list.size(); i++)
    {
        current_result = run_and_measure_time(test_list[i]);
        current_result.correctness = (Result == compare);
        results.push_back(current_result);
    }

    return results;
}

void dump_result(vector<TestResult> &cases)
{
    stringstream ss;

    // init tab width
    vector<int> tab_width(cases.size() + 1);
    int tail = 0;
    tab_width[0] = 12; // magic number, length of "Worker(s)"
    for (int i = 0; i < cases.size(); i++)
        tab_width[i + 1] = max((int)cases[i].name.length() + 2, 10);
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