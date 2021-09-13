#include "utils.hpp"
#include "test.hpp"

extern string TimingResult;

int main(int argc, char **argv)
{
    if (!parse_args(argc, argv))
    {
        cerr << "Usage: page-rank <filename>" << endl;
        exit(1);
    }

    cout << "============================================================" << endl;
    cout << "===========      Algorithm Engineering Lab      ============" << endl;
    cout << "============================================================" << endl;
    cout << "Project 3: Page Rank" << endl;
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
    vector<TestResult> test_result = test_all();
    cout << endl << "------------------------------------------------------------" << endl;
    dump_result(test_result);
    cout << TimingResult;
    cout << "============================================================" << endl;
}
