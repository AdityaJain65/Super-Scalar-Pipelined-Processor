#include <bits/stdc++.h>
using namespace std;
#pragma once
// can be a error?
class ArchitectureReg;
class PhysicalReg
{
public:
   bool busy;
   bool valid;
   int data;
   int ArchitectureTag;
   PhysicalReg(bool busy = false, bool valid = false, int data = -1, int ArchitectureTag = -1)
   {
      this->busy = busy;
      this->valid = valid;
      this->data = data;
      this->ArchitectureTag = ArchitectureTag;
   }
   void regupd(vector<ArchitectureReg> &architecture);
};
class ArchitectureReg
{
public:
   bool busy;
   int PhysicalTag;
   int data;
   int index;
   ArchitectureReg(bool busy = false, int PhysicalTag = -1, int data = -1, int index = -1)
   {
      this->busy = busy;
      this->PhysicalTag = PhysicalTag;
      this->data = data;
      this->index = index;
   }
   int destination_allocate(vector<PhysicalReg> &physical);
   pair<bool, int> source_read(vector<PhysicalReg> &physical);
};