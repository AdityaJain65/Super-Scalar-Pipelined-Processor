#include <bits/stdc++.h>
#include "DecodedIns.h"
using namespace std;
#pragma once
class ResStation
{
public:
    DecodedIns I;
    bool busy;
    bool ready;
    bool finished;
    bool executed;
    ResStation()
    {
        I = DecodedIns();
        busy = false;
        ready = false;
        finished = false;
        executed = false;
    }
};