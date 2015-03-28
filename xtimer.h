#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class CXTimer 
{


public:

	__int64 Freq;
  __int64 Start;
  CXTimer();
 void InitFreq();
  void Reset();
   double GetElapsed();
   double GetElapsedSec();
   double GetFreq();
};