#include <bits/stdc++.h>
#include "ResStation.h"
#include "TrackIns.h"
using namespace std;
#pragma once
class Arithmetic;
class Logical;
class Memory;
class Branch;
class FunctionalUnit
{
public:
   vector<ResStation> resStations;
   FunctionalUnit()
   {
      resStations.resize(2);
   }
   void validateTag(int tag, vector<PhysicalReg> &phy);
};