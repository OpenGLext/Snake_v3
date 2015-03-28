#pragma once
#include <fstream>
using namespace std;

std::ofstream LogProfiler;
char *nf_ = "C:\\LogProfiler.txt";

void init()
{
	LogProfiler.open(nf_,ios::out);
}

__int64 GetCPUClock()
{
    __int64 res;
    __asm
    {
        rdtsc
        mov dword ptr res, eax
        mov dword ptr res+4, edx
    }
    return res;
}

__int64 g_FuncTime = 0; //сюда будет положено суммарное время выполнения F.

void F()
{
    __int64 Time = GetCPUClock();
    ///...
    __int64 Difference = GetCPUClock() - Time;
    g_FuncTime += Difference;
}