#include "utils.hpp"
#include "test.hpp"
#include "adjoint_list.hpp"
#include "solution.hpp"

AdjointList Source;
Solution TheSolution;
string timing_result = "";

vector<TestCase> test_list =
{
};

bool initialize(int argc, char **argv)
{
    bool is_binary, is_text;
    if (argc < 2)
    {
        cerr << "Usage: page-rank <filename>" << endl;
        return false;
    }

    bool load_result = Source.load_data_binary(argv[1]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }

    TheSolution.attach_to_graph(Source);
    return true;
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
    const int TAB_WIDTH = 12;
    // cout << left;
    ss << left;
    
    if (1 == cases[0].nworkers)
    {
        // cout << setw(TAB_WIDTH) << "Workers";
        ss << setw(TAB_WIDTH) << "Workers";
        for (int i = 0; i < cases.size(); i++)
            // cout << setw(TAB_WIDTH) << cases[i].name;
            ss << setw(TAB_WIDTH) << cases[i].name;
        // cout << endl;
        ss << endl;
    }

    // cout << setw(TAB_WIDTH) << __cilkrts_get_nworkers();
    ss << setw(TAB_WIDTH) << __cilkrts_get_nworkers();
    for (int i = 0; i < cases.size(); i++)
    {
        // cout << setw(TAB_WIDTH);
        ss << setw(TAB_WIDTH);
        if (!cases[i].correctness)
        {
            // cout << "Wrong Ans";
            ss << "Wrong Ans";
        }
        else if (NAN == cases[i].time)
        {
            // cout << "#Error";
            ss << "#Error";
        }
        else
            // cout << cases[i].time;
            ss << cases[i].time;
    }
    // cout << endl;
    ss << endl;

    timing_result += ss.str();
}