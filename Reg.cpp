#include "Reg.h"
// different
// make update function
int ArchitectureReg::destination_allocate(vector<PhysicalReg> &physical)
{
    for (int i = 0; i < 32; i++)
    {
        if (!physical[i].busy)
        {
            this->PhysicalTag = i;
            this->busy = true;
            physical[i].ArchitectureTag = index;
            physical[i].busy = true;
            physical[i].valid = false;
            return i;
            break;
        }
    }
}
pair<bool, int> ArchitectureReg::source_read(vector<PhysicalReg> &physical)
{
    if (!this->busy)
    {
        return {true, this->data};
    }
    else if (physical[PhysicalTag].valid)
    {
        return {true, physical[PhysicalTag].data};
    }
    else
    {
        return {false, PhysicalTag};
    }
}
void PhysicalReg::regupd(vector<ArchitectureReg> &architecture)
{
    int index = ArchitectureTag;
    architecture[index].data = this->data;
    architecture[index].busy = false;
    this->busy = false;
}