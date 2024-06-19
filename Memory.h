#include <bits/stdc++.h>
#include "FunctionalUnit.h"
using namespace std;
#pragma once
class Memory : public FunctionalUnit
{
public:
    Memory() : FunctionalUnit()
    {
    }
    void FU(ResStation &R, vector<int> &DCache);
    void process(vector<TrackIns> &order, vector<int> &DCache, int &data_stalls);
};