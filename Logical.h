#include <bits/stdc++.h>
#include "FunctionalUnit.h"
#pragma once
class Logical : public FunctionalUnit
{
public:
    Logical() : FunctionalUnit()
    {
    }
    void FU(ResStation &R);
    void process(vector<TrackIns> &order, int &data_stalls);
};