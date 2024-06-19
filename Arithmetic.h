#include <bits/stdc++.h>
#include "FunctionalUnit.h"
using namespace std;
#pragma once
class Arithmetic : public FunctionalUnit
{
public:
    Arithmetic() : FunctionalUnit()
    {
    }
    void FU(ResStation &R);
    void process(vector<TrackIns> &order, int &data_stalls);
};