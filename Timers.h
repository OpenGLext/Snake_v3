#pragma once
#include "StructData.h"

void glutTimer(int id)

{
	if (id!=0)
	{
	  Draw(); 
	  //ProcessMenu();
	}
	glutTimerFunc(Data::IntervalTimer,glutTimer,id);

}

