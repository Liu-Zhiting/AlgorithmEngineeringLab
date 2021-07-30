#include "utils.hpp"
#include "test.hpp"
#include "array.hpp"

Array Source, Result;

vector<TestCase> test_list_new =
{
    {"merge_sort", merge_sort},
    {"quick_sort", quick_sort}
};

bool initialize(int argc, char **argv)
{
    bool is_binary, is_text;
    if (argc < 2)
    {
        cerr << "Usage: sort <filename>" << endl;
        return false;
    }

    bool load_result = Source.load_data_binary(argv[1]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }
    Result = Source;

    return true;
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

    TestCase ref_case = {"ref_sort", ref_sort};
    TestResult ref_result = run_and_measure_time(ref_case);
    Array compare(Result);
    ref_result.correctness = true;
    results.push_back(ref_result);

    for (int i = 0; i < test_list_new.size(); i++)
    {
        current_result = run_and_measure_time(test_list_new[i]);
        current_result.correctness = (Result == compare);
        results.push_back(current_result);
    }

    return results;
}

void dump_result(vector<TestResult> &cases)
{
    const int TAB_WIDTH = 12;
    cout << left;
    if (1 == cases[0].nworkers)
    {
        cout << setw(TAB_WIDTH) << "Workers";
        for (int i = 0; i < cases.size(); i++)
            cout << setw(TAB_WIDTH) << cases[i].name;
        cout << endl;
    }

    cout << setw(TAB_WIDTH) << __cilkrts_get_nworkers();
    for (int i = 0; i < cases.size(); i++)
    {
        cout << setw(TAB_WIDTH);
        if (!cases[i].correctness)
        {
            cout << "Wrong Ans";
        }
        else if (NAN == cases[i].time)
        {
            cout << "#Error";
        }
        else
            cout << cases[i].time;
    }
    cout << endl;
}