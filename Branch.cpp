#include <bits/stdc++.h>
#include "Branch.h"
using namespace std;
void Branch::process(vector<TrackIns> &order, int &data_stalls)
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
            else
            {
                data_stalls++;
            }
        }
    }
}
void Branch::FU(ResStation &R)
{
    DecodedIns &I = R.I;
    switch (I.op)
    {
    case 11:
        if (I.RA == 0)
            I.Condition = true;
        else
            I.Condition = false;
    case 10:
        I.AluOutput = I.NPC + (I.Imm << 1);
        // cout << I.op << " " << I.AluOutput << endl;
    }
    R.executed = 1;
}