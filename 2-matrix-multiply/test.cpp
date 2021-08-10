#include "utils.hpp"
#include "test.hpp"
#include "matrix.hpp"

Matrix A, B, C;
string FileNameA = "";
string FileNameB = "";
string TimingResult = "";

vector<TestCase> test_list =
    {
        {"loop", loop},
        {"divide_and_conquer", divide_and_conquer},
        {"strassen", strassen}};

bool parse_args(int argc, char **argv)
{
    if (argc < 3)
        return false;
    FileNameA = argv[1];
    FileNameB = argv[2];
    return true;
}

bool initialize()
{
    bool load_result = A.load_data_binary(FileNameA.c_str());
    if (!load_result)
        return false;
    load_result = B.load_data_binary(FileNameB.c_str());
    if (!load_result)
        return false;

    if (A.get_size() != B.get_size())
    {
        cerr << "Error: A.size != B.size" << endl;
        return false;
    }

    C.resize(A.get_size());
    return true;
}

void print_data_info()
{
    cout << "Data Info: " << endl
         << "Size: " << A.get_size() << 'x' << A.get_size() << endl;
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
        C.multiply_by_func(A, B, test_case.func);
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