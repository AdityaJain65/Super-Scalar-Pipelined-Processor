#include <bits/stdc++.h>
#include "FunctionalUnit.h"
using namespace std;
void FunctionalUnit::validateTag(int tag, vector<PhysicalReg> &phy)
{
    for (int i = 0; i < 2; i++)
    {
        if (resStations[i].busy)
        {
            DecodedIns &I = resStations[i].I;
            if (!I.validA)
            {
                // cout << "*" << " " << I.RA << " " << tag << " " << I.op << endl;
                if (I.RA == tag)
                {
                    I.RA = phy[tag].data;
                    I.validA = true;
                }
            }
            if (!I.validB)
            {
                if (I.RB == tag)
                {
                    I.RB = phy[tag].data;
                    I.validB = true;
                }
            }
        }
    }
}
