#pragma once
#include "utils.hpp"

class TSPMap
{
private:
    double *x, *y;
    int n;
    void dispose();
    string name;

public:
    TSPMap() : x(nullptr), y(nullptr), n(0), name("") {}
    ~TSPMap() { dispose(); }
    bool LoadMap(string filename);
    double getTourDistance(const vector<int>& answer) const;
    string getName() const { return name; }
    int getNodeCount() const { return n; }
    bool isAnswerValid(const vector<int>& answer) const;
};