#include "utils.hpp"

int main(int argc, char **argv)
{
    if (!parse_args(argc, argv))
    {
        cerr << "Usage: tspvd <dataset-name> <answer-file-name>" << endl;
        cerr << "Available DataSet:" << endl;
        printDataSetNames();
        exit(1);
    }

    cout << "============================================================" << endl;
    cout << "===========      Algorithm Engineering Lab      ============" << endl;
    cout << "============================================================" << endl;
    cout << "Project 6: TSP Validator" << endl;
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

    runCmp();

    cout << "============================================================" << endl;
}
