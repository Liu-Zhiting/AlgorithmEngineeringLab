#include "utils.hpp"
#include "test.hpp"
#include "adjoint_list.hpp"

Graph Source;
// Solution TheSolution;
uint32_t Result = 0;

vector<fun_ptr> test_list =
    {

};

bool initialize(int argc, char **argv)
{
    bool is_binary, is_text;
    if (argc < 3)
    {
        cerr << "Usage: <path_to_graph> <file_type:text|binary>" << endl;
        return false;
    }
    else
    {
        is_binary = !strcmp(argv[2], "binary");
        is_text = !strcmp(argv[2], "text");
    }

    if ((!is_binary && !is_text) || (is_binary && is_text))
    {
        cerr << " Invalid argument <file_type>: " << argv[2] << endl;
        return false;
    }

    bool load_result;
    load_result = false;
    if (is_binary)
        load_result = Source.load_data_binary(argv[1]);
    if (is_text)
        load_result = Source.load_data_text(argv[1]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }

    // TheSolution.attach_to_graph(Source);
    return true;
}

TestResult run_and_measure_time(fun_ptr func)
{
    TestResult tc;
    tc.func = func;
    tc.name = "[unknown]";
    tc.nworkers = __cilkrts_get_nworkers();
    try
    {
        // TheSolution.clear();
        Result = 0;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        string func_name = func(Source, Result);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        tc.name = func_name;
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
    vector<TestResult> cases;
    TestResult current_case;

    TestResult ref_case = run_and_measure_time(ref);
    // Solution compare(TheSolution);
    uint32_t compare = Result;
    ref_case.correctness = true;
    cases.push_back(ref_case);
    for (int i = 0; i < test_list.size(); i++)
    {
        current_case = run_and_measure_time(test_list[i]);
        current_case.correctness = (Result == compare);
        cases.push_back(current_case);
    }

    return cases;
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