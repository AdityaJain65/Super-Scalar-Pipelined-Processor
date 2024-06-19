#include <bits/stdc++.h>
#include "DecodedIns.h"
using namespace std;
#pragma once
class TrackIns
{
public:
    DecodedIns I;
    bool busy;
    bool issued;
    bool finished;
    int IA;
    TrackIns()
    {
        busy = false;
        issued = false;
        finished = false;
    }
};