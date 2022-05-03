#include "utils.hpp"
#include "tspmap.hpp"
#include <map>

using namespace std;

TSPMap Source;
string DataSetName = "";
string FileName = "";
vector<int> answer;

typedef struct DataSetInfo
{
    string name;
    string path;
    double optAns;
} DataSetInfo;

map<string, DataSetInfo> DataSet = 
{
    {"wi29", {
        "wi29",
        "./data/wi29.tsp", 
        27603.0
    }},
        {"lu980", {
        "lu980",
        "./data/lu980.tsp", 
        11340.0
    }},
        {"nu3496", {
        "nu3496",
        "./data/nu3496.tsp", 
        96132.0
    }},
        {"eg7146", {
        "eg7146",
        "./data/eg7146.tsp", 
        172387.0
    }},
        {"ch71009", {
        "ch71009",
        "./data/ch71009.tsp", 
        4566563.0
    }}
};

void printDataSetNames()
{
    for(auto i = DataSet.begin(); i!=DataSet.end(); i++)
        cout << i->first << endl;
}

bool parse_args(int argc, char **argv)
{
    if (argc < 3)
        return false;
    DataSetName = argv[1];
    FileName = argv[2];
    cout << DataSetName << "\n" << FileName << endl;
    return true;
}

bool initialize()
{
    // init map
    if (DataSet.find(DataSetName) == DataSet.end())
        return false;

    bool isMapLoadSucceed = Source.LoadMap(DataSet.at(DataSetName).path);
    if (!isMapLoadSucceed)
        return false;

    // init ans
    answer.assign(Source.getNodeCount(), -1);
    ifstream fin(FileName);
    if (!fin)
        return false;
    for (int i = 0; i < Source.getNodeCount(); i++)
        fin >> answer[i];
    fin.close();
    return true;
}

void print_data_info()
{
    cout << "Data Info: " << endl
         << "Name: " << Source.getName() << endl
         << "Node Count: " << Source.getNodeCount() << endl;
}

void runCmp()
{
    // check if answer valid
    if(!Source.isAnswerValid(answer))
    {
        cout << "Your answer is invalid, please check again." << endl;
        return;
    }

    // benchmark
    double result = Source.getTourDistance(answer);
    double delta = result - DataSet.at(DataSetName).optAns;
    double rate = DataSet.at(DataSetName).optAns / result * 100;
    cout << fixed << setprecision(2);
    cout << "Your answer builds a tour of " << result << endl
         << "The optimized distance is " << DataSet.at(DataSetName).optAns << endl
         << "Your answer gets " << delta << " longer distance than optimized" << endl
         << "Your answer is approximately " << rate << "% of optimized" << endl;
}
