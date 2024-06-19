#include "Instruction.h"
Instruction::Instruction(int IR, int index)
{
    this->IR = IR;
    this->index = index;
}
void Instruction::SetIR(int IR)
{
    this->IR = IR;
}
void Instruction::SetIndex(int index)
{
    this->index = index;
}