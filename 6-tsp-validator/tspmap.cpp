#include "tspmap.hpp"

void TSPMap::dispose()
{
    if (x != nullptr)
    {
        delete[] x;
        x = nullptr;
    }
    if (y != nullptr)
    {
        delete[] y;
        y = nullptr;
    }
}

bool TSPMap::LoadMap(string filename)
{
    // init file stream
    ifstream fin;
    fin.open(filename);
    if (!fin)
        return false;

    // init tmp var
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    char *tmp = nullptr;
    bool content = false;
    int index;

    // parse file
    while (true)
    {
        fin.getline(buffer, BUFFER_SIZE);

        // EOF
        if (nullptr != strstr(buffer, "EOF"))
        {
            break;
        }

        if (content)
        {
            tmp = strtok(buffer, " ");
            index = atoi(tmp) - 1;
            tmp = strtok(NULL, " ");
            x[index] = stod(tmp);
            tmp = strtok(NULL, " ");
            y[index] = stod(tmp);
            continue;
        }

        // NAME : ch71009
        if (nullptr != (tmp = strstr(buffer, "NAME : ")))
        {
            tmp += 7;
            name = string(tmp);
            continue;
        }

        // COMMENT : 71009 locations in China
        if (nullptr != strstr(buffer, "COMMENT : "))
        {
            continue;
        }

        // TYPE : TSP
        if (nullptr != strstr(buffer, "TYPE : "))
        {
            continue;
        }

        // DIMENSION : 71009
        if (nullptr != (tmp = strstr(buffer, "DIMENSION : ")))
        {
            tmp += 12;
            n = atoi(tmp);
            x = new double[n];
            y = new double[n];
            continue;
        }

        // EDGE_WEIGHT_TYPE : EUC_2D
        if (nullptr != strstr(buffer, "EDGE_WEIGHT_TYPE :"))
        {
            continue;
        }

        // NODE_COORD_SECTION
        if (nullptr != strstr(buffer, "NODE_COORD_SECTION"))
        {
            content = true;
            continue;
        }
    }

    // close file
    fin.close();
    return true;
}

double TSPMap::getTourDistance(const vector<int> &answer) const
{
    double result = 0.0, dx, dy;
    int i, j;
    int length = answer.size();

    for (int iter = 1; iter < length; iter++)
    {
        i = answer.at(iter - 1);
        j = answer.at(iter);
        dx = x[j] - x[i];
        dy = y[j] - y[i];
        result += sqrt(dx * dx + dy * dy);
    }

    // add distance between point n-1 and point 0
    i = answer.at(length - 1);
    j = answer.at(0);
    dx = x[j] - x[i];
    dy = y[j] - y[i];
    result += sqrt(dx * dx + dy * dy);

    return result;
}

bool TSPMap::isAnswerValid(const vector<int> &answer) const
{
    if (answer.size() != n)
        return false;
    vector<bool> a;
    a.assign(n, false);
    int i;
    try
    {
        for (i = 0; i < n; i++)
            a.at(answer.at(i)) = true;
    }
    catch (out_of_range e)
    {
        return false;
    }

    for (i = 0; i < n; i++)
        if (!a.at(i))
            return false;
    return true;
}