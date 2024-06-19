#include <bits/stdc++.h>
#include "Logical.h"
using namespace std;
void Logical::process(vector<TrackIns> &order, int &data_stalls)
{
    for (int i = 0; i < resStations.size(); i++)
    {
        ResStation &R = resStations[i];
        if (R.busy)
        {
            DecodedIns &I = R.I;
            if (I.validA && I.validB)
            {
                R.ready = true;
                FU(R);
                for (int i = 0; i < order.size(); i++)
                {
                    if (order[i].IA == I.index)
                    {
                        order[i].issued = true;
                    }
                }
            }
        }
        else
        {
            data_stalls++;
        }
    }
}
void Logical::FU(ResStation &R)
{
    DecodedIns &I = R.I;
    switch (I.op)
    {
    case 4:
        I.AluOutput = I.RA & I.RB;
        break;
    case 5:
        I.AluOutput = I.RA | I.RB;
        break;
    case 6:
        I.AluOutput = ~I.RA;
        break;
    case 7:
        I.AluOutput = I.RA ^ I.RB;
        break;
    }
    R.executed = 1;
}