#include <bits/stdc++.h>
#include "DecodedIns.h"
using namespace std;
void DecodedIns::hex_to_dec(int &x) // Converts HEX to DEC
{
    if (x & (1 << 7))
        x = x - (1 << 8);
}
void DecodedIns::decode(vector<ArchitectureReg> &arch, vector<PhysicalReg> &phy, bool &halt, bool &can_fetch)
{
    int temp1 = IR;
    RS2 = (temp1 & 15);
    temp1 = temp1 >> 4;
    RS1 = (temp1 & 15);
    temp1 = temp1 >> 4;
    RD = (temp1 & 15);
    temp1 = temp1 >> 4;
    op = (temp1 & 15);
    pair<bool, int> temp;
    switch (op)
    {
    case 0: // ADD instruction
    case 1: // SUB instruction
    case 2: // MUL instruction
    case 4: // AND instruction
    case 5: // OR instruction
    case 7: // XOR instruction
        temp = arch[RS1].source_read(phy);
        validA = temp.first;
        RA = temp.second;
        temp = arch[RS2].source_read(phy);
        validB = temp.first;
        RB = temp.second;
        DestTag = arch[RD].destination_allocate(phy);
        break;
    case 3: // INC instruction
        temp = arch[RS1].source_read(phy);
        validA = temp.first;
        RA = temp.second;
        validB = 1;
        RB = 1;
        DestTag = arch[RD].destination_allocate(phy);
        break;
    case 6: // NOT instruction
        temp = arch[RS1].source_read(phy);
        validA = temp.first;
        RA = temp.second;
        validB = 1;
        RB = 0xff;
        DestTag = arch[RD].destination_allocate(phy);
        break;
    case 8: // LOAD instruction
        temp = arch[RS1].source_read(phy);
        validA = temp.first;
        RA = temp.second;
        validB = 1;
        Imm = RS2;
        hex_to_dec(Imm);
        DestTag = arch[RD].destination_allocate(phy);
        break;
    case 9: // STORE instruction
        temp = arch[RS1].source_read(phy);
        validA = temp.first;
        RA = temp.second;
        temp = arch[RD].source_read(phy);
        validB = temp.first;
        RB = temp.second;
        Imm = RS2;
        hex_to_dec(Imm);
        break;
    case 10: // JMP instruction
        Imm = (RD << 4) + RS1;
        validA = 1;
        validB = 1;
        hex_to_dec(Imm);
        can_fetch = 0;
        break;
    case 11: // BEQZ instruction
        temp = arch[RD].source_read(phy);
        validA = temp.first;
        RA = temp.second;
        Imm = (RS1 << 4) + RS2;
        validB = 1;
        hex_to_dec(Imm);
        can_fetch = 0;
        break;
    case 15: // HLT instruction
        halt = 1;
        break;
    }
    // cout<<validA<<" "<<validB<<endl;
}