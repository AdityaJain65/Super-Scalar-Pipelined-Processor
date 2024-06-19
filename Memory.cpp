#include <bits/stdc++.h>
#include "Memory.h"
using namespace std;
void Memory::process(vector<TrackIns> &order, vector<int> &DCache, int &data_stalls)
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
                FU(R, DCache);
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
void Memory::FU(ResStation &R, vector<int> &DCache)
{
    DecodedIns &I = R.I;
    switch (I.op)
    {
    case 8:
    {
        I.AluOutput = I.RA + I.Imm;
        I.LMD = DCache[I.AluOutput];
        // cout<<I.LMD<<endl;
        break;
    }
    case 9:
        I.AluOutput = I.RA + I.Imm;
        break;
    }
    R.executed = 1;
}