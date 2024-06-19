#include <bits/stdc++.h>
#include "Instruction.h"
#include "Reg.h"
using namespace std;
#pragma once
class DecodedIns : public Instruction
{
public:
    int op;
    int RD;
    int RS1;
    int RS2;
    bool validA;
    bool validB;
    int RA;
    int RB;
    int DestTag;
    int Imm;
    int AluOutput;
    int LMD;
    bool Condition;
    DecodedIns(int IR = -1, int index = -1) : Instruction(IR, index)
    {
        int temp1 = IR;
        RS2 = (temp1 & 15);
        temp1 = temp1 >> 4;
        RS1 = (temp1 & 15);
        temp1 = temp1 >> 4;
        RD = (temp1 & 15);
        temp1 = temp1 >> 4;
        op = (temp1 & 15);
        Condition = false;
    }
    void decode(vector<ArchitectureReg> &arch, vector<PhysicalReg> &phy, bool &halt, bool &can_fetch);
    void hex_to_dec(int &x); // Converts HEX to DEC
};