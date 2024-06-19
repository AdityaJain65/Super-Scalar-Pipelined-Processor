#include <bits/stdc++.h>
#include <fstream>
#include "Arithmetic.cpp"
#include "Logical.cpp"
#include "Memory.cpp"
#include "Branch.cpp"
#include "DecodedIns.cpp"
#include "FunctionalUnit.cpp"
#include "Instruction.cpp"
#include "Reg.cpp"
using namespace std;
int PC = 0;
int maxRRFe = 32, maxfetch = 4;
vector<int> ICache(256, 0);
vector<int> DCache(256, 0);
vector<int> RF(16, 0);
int num_stall = 0, num_dat_st = 0, num_con_st = 0, num_inst = 0, num_cycles = 0; // Statistics
int num_arith = 0, num_log = 0, num_con = 0, num_data = 0, num_halt = 0;         // Statistics
bool halt = false, can_fetch = true;
vector<Instruction> IBuf;
vector<DecodedIns> DBuf;
vector<ArchitectureReg> ArchReg(16);
vector<PhysicalReg> PhyReg(32);
vector<TrackIns> order;
Arithmetic A;
Logical L;
Memory M;
Branch B;
void read_input(vector<int> &vec, int N, string path)
{
    ifstream ifile;
    ifile.open(path);
    for (int i = 0; i < N; i++)
    {
        ifile >> hex >> vec[i];
    }
}
void stats(int op)
{
    num_inst++;
    // cout << num_inst << " " << op << endl;
    if (op < 4)
        num_arith++;
    else if (op < 8)
        num_log++;
    else if (op < 10)
        num_data++;
    else if (op < 12)
        num_con++;
    else
        num_halt++;
}
void print_output(vector<int> &arr, int size, string filename)
{ // Print Output
    ofstream output;
    output.open(filename);
    for (int i = 0; i < size; i++)
    {
        int temp = arr[i];
        temp = temp & 0xff;
        output << hex << (temp >> 4);
        temp = temp & 0xf;
        output << hex << temp << endl;
    }
    output.close();
}

void print_stats(string filename)
{ // Print Stats
    ofstream output;
    output.open(filename);
    // output << num_cycles << endl;
    output << "Total number of instructions executed:" << num_inst << endl;
    output << "Number of instructions in each class" << endl;
    output << "Arithmetic instructions              :" << num_arith << endl;
    output << "Logical instructions                 :" << num_log << endl;
    output << "Data instructions                    :" << num_data << endl;
    output << "Control instructions                 :" << num_con << endl;
    output << "Halt instructions                    :" << num_halt << endl;
    output << "Cycles Per Instruction               :" << (double)num_cycles / num_inst << endl;
    output << "Total number of stalls               :" << num_stall << endl;
    output << "Data stalls (RAW)                    :" << num_dat_st << endl;
    output << "Control stalls                       :" << num_con_st << endl;
    output.close();
}
void initialize()
{
    for (int i = 0; i < 16; i++)
    {
        ArchReg[i].index = i;
        ArchReg[i].data = RF[i];
    }
}
void update(int result, DecodedIns &I)
{
    PhyReg[I.DestTag].data = result;
    PhyReg[I.DestTag].valid = true;
    A.validateTag(I.DestTag, PhyReg);
    L.validateTag(I.DestTag, PhyReg);
    M.validateTag(I.DestTag, PhyReg);
    B.validateTag(I.DestTag, PhyReg);
    for (int i = 0; i < DBuf.size(); i++)
    {
        DecodedIns &DI = DBuf[i];
        if (!DI.validA)
        {
            if (DI.RA == I.DestTag)
            {
                DI.RA = PhyReg[DI.RA].data;
                DI.validA = true;
            }
        }
        if (!DI.validB)
        {
            if (DI.RB == I.DestTag)
            {
                DI.RB = PhyReg[DI.RB].data;
                DI.validB = true;
            }
        }
    }
}
void finish(FunctionalUnit &A)
{
    for (int i = 0; i < 2; i++)
    {
        if (A.resStations[i].executed)
        {
            A.resStations[i].busy = false;
            A.resStations[i].executed = false;
            DecodedIns I = A.resStations[i].I;
            if (I.op < 8)
                update(I.AluOutput, I);
            else if (I.op == 8)
            {
                update(I.LMD, I);
            }
            A.resStations[i].finished = true;
            for (int i = 0; i < order.size(); i++)
            {
                if (order[i].IA == I.index)
                {
                    order[i].I = I;
                    order[i].finished = true;
                }
            }
        }
    }
}
void Fetch()
{
    if (!halt && can_fetch)
    {
        for (int i = 0; i < maxfetch; i++)
        {
            if (PC >= ICache.size())
                break;
            int IR = (ICache[PC + 1] | (ICache[PC] << 8));
            int index = PC / 2;
            PC += 2;
            Instruction I(IR, index);
            I.NPC = PC;
            IBuf.push_back(I);
        }
    }
    else if (!can_fetch)
    {
        num_con_st++;
        num_stall++;
    }
}
void Decode()
{
    for (vector<Instruction>::iterator it = IBuf.begin(); it != IBuf.end();)
    {
        Instruction I = *it;
        DecodedIns DI(I.IR, I.index);
        DI.decode(ArchReg, PhyReg, halt, can_fetch);
        DI.NPC = I.NPC;
        DBuf.push_back(DI);
        IBuf.erase(it);
        if (DI.op > 9)
        {
            while (it != IBuf.end())
            {
                IBuf.erase(it);
            }
        }
    }
    // cout << DBuf.size() << endl;
}
void Dispatch()
{
    for (vector<DecodedIns>::iterator it = DBuf.begin(); it != DBuf.end();)
    {
        DecodedIns I = *it;
        int dispatched = 0;
        switch (I.op)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        {
            for (vector<ResStation>::iterator itt = A.resStations.begin(); itt != A.resStations.end(); itt++)
            {
                if (itt->busy == 0)
                {
                    itt->I = I;
                    itt->busy = 1;
                    itt->ready = 0;
                    dispatched = 1;
                    break;
                }
            }
            if (dispatched)
                DBuf.erase(it);
            break;
        }
        case 4:
        case 5:
        case 6:
        case 7:
        {
            for (vector<ResStation>::iterator itt = L.resStations.begin(); itt != L.resStations.end(); itt++)
            {
                if (itt->busy == 0)
                {
                    itt->I = I;
                    itt->busy = 1;
                    itt->ready = 0;
                    dispatched = 1;
                    break;
                }
            }
            if (dispatched)
                DBuf.erase(it);
            break;
        }
        case 8:
        case 9:
        {
            for (vector<ResStation>::iterator itt = M.resStations.begin(); itt != M.resStations.end(); itt++)
            {
                if (itt->busy == 0)
                {
                    itt->I = I;
                    itt->busy = 1;
                    itt->ready = 0;
                    dispatched = 1;
                    break;
                }
            }
            if (dispatched)
                DBuf.erase(it);
            break;
        }
        case 10:
        case 11:
        {
            for (vector<ResStation>::iterator itt = B.resStations.begin(); itt != B.resStations.end(); itt++)
            {
                if (itt->busy == 0)
                {
                    itt->I = I;
                    itt->busy = 1;
                    itt->ready = 0;
                    dispatched = 1;
                    break;
                }
            }
            if (dispatched)
                DBuf.erase(it);
            break;
        }
        case 15:
        {
            dispatched = 1;
            while (it != DBuf.end())
            {
                DBuf.erase(it);
            }
            break;
        }
        }
        if (dispatched)
        {
            TrackIns RRR;
            RRR.I = I;
            RRR.busy = true;
            RRR.IA = I.index;
            if (I.op == 15)
                RRR.finished = 1;
            order.push_back(RRR);
        }
        else
        {
            break;
        }
    }
}
void Execute()
{
    int prev_data_st = num_dat_st;
    A.process(order, num_dat_st);
    L.process(order, num_dat_st);
    M.process(order, DCache, num_dat_st);
    B.process(order, num_dat_st);
    num_stall += (num_dat_st - prev_data_st);
}
void Finish()
{
    finish(A);
    finish(L);
    finish(M);
    finish(B);
}
void Complete()
{
    for (vector<TrackIns>::iterator it = order.begin(); it != order.end();)
    {
        TrackIns &Re = *it;
        DecodedIns &I = Re.I;
        if (Re.finished)
        {
            switch (I.op)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            {
                PhyReg[I.DestTag].regupd(ArchReg);
                break;
            }
            case 9:
            {
                DCache[I.AluOutput] = I.RB;
                break;
            }
            case 10:
            {
                PC = I.AluOutput;
                can_fetch = 1;
                // cout << PC << endl;
                break;
            }
            case 11:
            {
                if (I.Condition)
                    PC = I.AluOutput;
                else
                    PC = I.NPC;
                can_fetch = 1;
                // cout << I.NPC << endl;
                // cout << PC << "*"<<endl;
                break;
            }
            case 15:
            {
                break;
            }
            }
            stats(I.op);
            order.erase(it);
        }
        else
        {
            break;
        }
    }
}
int main()
{
    read_input(ICache, 256, "./input/ICache.txt"); // Read ICache
    read_input(DCache, 256, "./input/DCache.txt"); // Read DCache
    read_input(RF, 16, "./input/RF.txt");          // Read RF
    initialize();

    do
    {
        num_cycles++;
        // Completion
        Complete();

        // Finishing
        Finish();

        // Issuing && Execution
        Execute();

        // Dispatch
        Dispatch();

        // Decode
        Decode();

        // Fetch
        Fetch();

    } while ((!IBuf.empty() || !DBuf.empty() || !order.empty() || !halt));

    for (int i = 0; i < 16; i++)
        RF[i] = ArchReg[i].data;

    print_output(DCache, 256, "./output/DCache.txt"); // Print DCache
    print_output(RF, 16, "./output/RF.txt");          // Print RF
    print_stats("./output/Output.txt");               // Print Stats
}