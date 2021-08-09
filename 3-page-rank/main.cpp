#include "utils.hpp"
#include "test.hpp"

extern string timing_result;

int main(int argc, char **argv)
{
    if (!initialize(argc, argv))
        return 1;

    char nworkers[10];
    int current_threads = 1;
    int max_threads = __cilkrts_get_nworkers();

    cout << "============================================================" << endl;
    cout << "===========      Algorithm Engineering Lab      ============" << endl;
    cout << "============================================================" << endl;
    cout << "Project 3: Page Rank" << endl
         << endl;
    while (current_threads <= max_threads)
    {
        sprintf(nworkers, "%d", current_threads);
        __cilkrts_set_param("nworkers", nworkers);
        vector<TestResult> test_result = test_all();
        dump_result(test_result);
        __cilkrts_end_cilk();
        current_threads <<= 1;
    }
    if ((current_threads >> 1) < max_threads)
    {
        sprintf(nworkers, "%d", current_threads);
        __cilkrts_set_param("nworkers", nworkers);
        vector<TestResult> test_result = test_all();
        dump_result(test_result);
    }

    cout << timing_result;

    cout << "============================================================" << endl;
}
