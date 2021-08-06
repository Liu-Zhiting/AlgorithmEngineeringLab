#include "utils.hpp"
#include "test.hpp"
#include "matrix.hpp"

Matrix A, B, C;

vector<TestCase> test_list =
{
    {"loop",loop},
    {"strassen",strassen},
    {"divide_and_conquer",divide_and_conquer}
};

bool initialize(int argc, char **argv)
{
    bool is_binary, is_text;
    if (argc < 3)
    {
        cerr << "Usage: matrix-mul <filename_A> <filename_B>"<< endl;
        return false;
    }

    bool load_result;
    load_result = A.load_data_binary(argv[1]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }

    load_result = false;
    load_result = B.load_data_binary(argv[2]);
    if (!load_result)
    {
        cerr << "Load failed" << endl;
        return false;
    }

    if (A.get_size() != B.get_size())
    {
        cerr << "A.size != B.size" << endl;
        return false;
    }

    C.resize(A.get_size());

    return true;
}

TestResult run_and_measure_time(TestCase test_case)
{
    TestResult test_result;
    test_result.name = test_case.name;
    test_result.nworkers = __cilkrts_get_nworkers();
    try
    {
        C.clear();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        // test_case.func(A, B, C);
        C.multiply_by_func(A,B,test_case.func);
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
    const Matrix compare(C);
    ref_result.correctness = true;
    results.push_back(ref_result);

    for (int i = 0; i < test_list.size(); i++)
    {
        current_result = run_and_measure_time(test_list[i]);
        current_result.correctness = (compare == C);
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