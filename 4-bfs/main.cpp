#include "utils.hpp"
#include "test.hpp"

extern string TimingResult;

void run_with_nworkers(int workers)
{
    char buf[10];
    sprintf(buf, "%d", workers);
    __cilkrts_set_param("nworkers", buf);
    cout << "Running with " << buf << " worker(s), output:" << endl;
    vector<TestResult> test_result = test_all();
    cout << endl << "------------------------------------------------------------" << endl;
    dump_result(test_result);
    __cilkrts_end_cilk();
}

int main(int argc, char **argv)
{
    if (!parse_args(argc, argv))
    {
        cerr << "Usage: bfs <filename>" << endl;
        exit(1);
    }

    int current_threads = 1;
    int max_threads = __cilkrts_get_nworkers();

    cout << "============================================================" << endl;
    cout << "===========      Algorithm Engineering Lab      ============" << endl;
    cout << "============================================================" << endl;
    cout << "Project 4: BFS" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Loading data . . ." << endl;
    if(!initialize())
    {
        cerr << "Load failed!" << endl;
        cout << "============================================================" << endl;
        exit(1);
    }
    cout << "Load succeeded!" << endl;
    print_data_info();
    cout << "------------------------------------------------------------" << endl;

    for (current_threads = 1; current_threads <= max_threads; current_threads <<= 1)
        run_with_nworkers(current_threads);
    if ((current_threads >> 1) < max_threads)
        run_with_nworkers(max_threads);

    cout << TimingResult;

    cout << "============================================================" << endl;
}
