#pragma once
#include "Timers.h"
#include "StructData.h"

using namespace Data;

void getkeys_down(unsigned char key,int x,int y)
{
	switch ( key )
	{

		// нужно использовать русскую раскладку проще действительно переключить раскладку
	case 'w' : snake.SetDir(0); break; //ClearScreen(); //Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break; 
	case 'a' : snake.SetDir(1); break; //ClearScreen(); //Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break; 
	case 'd' : snake.SetDir(2); break; //ClearScreen(); //Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break; 
	case 's' : snake.SetDir(3); break; //ClearScreen(); //Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break;

	case '1' : glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
	case '2' : glBlendFunc(GL_ONE, GL_ONE); break;
	case '3' : glBlendFunc(GL_DST_COLOR, GL_ZERO); break;
	case '4' : glBlendFunc(GL_SRC_COLOR, GL_ONE); break;
	case '5' : glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE); break;

	case 't' : Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break; 
	case 'e' : Data::id = 0; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break;
	case 'b' : blend = true; glEnable(GL_BLEND); glBlendFunc(GL_ZERO,  GL_SRC_ALPHA_SATURATE); break;

    case 'o' : depthFruct += stepdepth;  break;
	case 'p' : depthFruct -= stepdepth;  break;
	case 'i' : IntervalTimer += 100;  break;
	case 13  :  if ( CurMenu == LOGO ) { CurMenu = MAIN_MENU ;   } break;
	//case 32  : { CurMenu = MAIN_MENU ; ProcessMenu(); } break;

	case 'g' : CurMenu = GAME; Data::id = 1; break;

	case 27  : exit(0); break;

  
		// когда меняем направление на противоложное то получается столкновение с самим собой !!! ошибка исправить

	};
}