#include "utils.hpp"

int main(int argc, char **argv)
{
    if (!initialize(argc, argv))
        return 1;

    char nworkers[10];
    int current_threads = 1;
    int max_threads = __cilkrts_get_nworkers();

    cout << "============================================================" << endl;
    cout << "==============      Parallel Course Lab      ===============" << endl;
    cout << "============================================================" << endl;
    cout << "Project 2: Matrix multiply" << endl
         << endl;
    while (current_threads <= max_threads)
    {
        sprintf(nworkers, "%d", current_threads);
        __cilkrts_set_param("nworkers", nworkers);
        vector<TestCase> test_result = test_all();
        dump_result(test_result);
        __cilkrts_end_cilk();
        current_threads <<= 1;
    }
    if ((current_threads >> 1) < max_threads)
    {
        sprintf(nworkers, "%d", current_threads);
        __cilkrts_set_param("nworkers", nworkers);
        vector<TestCase> test_result = test_all();
        dump_result(test_result);
    }

    cout << "============================================================" << endl;
}
