#include "xtimer.h"

CXTimer::CXTimer()
{
  this->InitFreq();
  this->Reset();
}

void CXTimer::InitFreq()
{
  QueryPerformanceFrequency((PLARGE_INTEGER) &Freq);
  QueryPerformanceCounter((PLARGE_INTEGER) &Start);
}

void CXTimer::Reset()
{
  QueryPerformanceCounter((PLARGE_INTEGER) &Start);
}

 double CXTimer::GetElapsed()
{
  __int64 Now;
  QueryPerformanceCounter((PLARGE_INTEGER) &Now);
  return (double) (Now - Start) / GetFreq();
}

 double CXTimer::GetElapsedSec()
{
  return GetElapsed() / GetFreq();
}

 double CXTimer::GetFreq()
{
  return (double) Freq;
}