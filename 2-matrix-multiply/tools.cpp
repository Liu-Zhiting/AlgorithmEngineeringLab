#include "utils.hpp"
#include "test.hpp"
#include "matrix.hpp"

MatArray A, B, Result;

vector<fun_ptr> test_list =
    {
        loop,
        strassen,
        parallel_d2c};

bool initialize(int argc, char **argv)
{
    bool is_binary, is_text;
    if (argc < 4)
    {
        cerr << "Usage: <path_to_matrixA> <path_to_matrixB> "
                "<file_type:text|binary>"
             << endl;
        return false;
    }
    else
    {
        is_binary = !strcmp(argv[3], "binary");
        is_text = !strcmp(argv[3], "text");
    }

    if ((!is_binary && !is_text) || (is_binary && is_text))
    {
        cerr << " Invalid argument <file_type>: " << argv[3] << endl;
        return false;
    }

    bool load_result;
    load_result = false;
    if (is_binary)
        load_result = A.load_data_binary(argv[1]);
    if (is_text)
        load_result = A.load_data_text(argv[1]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }

    load_result = false;
    if (is_binary)
        load_result = B.load_data_binary(argv[2]);
    if (is_text)
        load_result = B.load_data_text(argv[2]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }

    if (A.size != B.size)
    {
        cerr << "A.size != B.size" << endl;
        return false;
    }

    Result.resize(A.size);

    return true;
}

TestCase run_and_measure_time(fun_ptr func)
{
    TestCase tc;
    tc.func = func;
    tc.name = "[unknown]";
    tc.nworkers = __cilkrts_get_nworkers();
    try
    {
        Result.clear();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        string func_name = func(A, B, Result);
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

vector<TestCase> test_all()
{
    vector<TestCase> cases;
    TestCase current_case;

    TestCase ref_case = run_and_measure_time(ref);
    MatArray compare(Result);
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

void dump_result(vector<TestCase> &cases)
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