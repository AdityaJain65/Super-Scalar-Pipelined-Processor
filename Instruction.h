#include <bits/stdc++.h>
using namespace std;
#pragma once
class Instruction
{
public:
    int IR;
    int index;
    int NPC;
    Instruction(int IR = -1, int index = -1);
    void SetIR(int IR);
    void SetIndex(int index);
};