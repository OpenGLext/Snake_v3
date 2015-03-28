/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#pragma once
#include "GunTypes.h"

//Класс таймера
class gunTimer
{
public:
	gunTimer(void);
	~gunTimer(void);
	//Для хранения времени
	UINT64 StartTime;
	//Прошедшее время
	UINT64 ElapsedTime;
	//Сохраняем время
	void gunStartTiming();
	//Считаем разницу
	UINT64 gunTimeElapsed();
};

//Текущее значения счетчика
UINT64 gunTime();
//Возвращаем в тиках
UINT64 gunGetTicksTime();
//Конвертируем в секундах
double gunConvertTicksToSeconds(UINT64 ticks);
// Инициализация
void gunInitTimeOperation();