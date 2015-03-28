#pragma once
#include "D:\\Движки\freeglut-2.8.1\include\GL\freeglut.h"

void mouse_select(int button, int state, int x, int y)
{
	std::stringstream ss;
  
    ss<<x;
    std::string str;
    ss>>str;

	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{ 
		

	} 

	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		
		
			
	}
}