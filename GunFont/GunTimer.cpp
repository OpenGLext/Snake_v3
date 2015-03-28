/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#include "guntimer.h"

gunTimer::gunTimer(void)
{

}

gunTimer::~gunTimer(void)
{
}

//Частота биения таймера
UINT64 freq;


//Инициализируем все операции со временем
void gunInitTimeOperation()
{
 LARGE_INTEGER s;
 //получаем частоту
 QueryPerformanceFrequency(&s);
 //сохраняем её
 freq=s.QuadPart;
}
//Текущее значение глобального счетчика
UINT64 gunTime()
{
 LARGE_INTEGER s;
 //Получаем счетчик
 QueryPerformanceCounter(&s);
 //Возвращаем только четвёртую часть
 return s.QuadPart;
}
  
//Возвращаем время в тиках
UINT64 gunGetTicksTime()
{
 LARGE_INTEGER s;
 //Получаем счетчик
 QueryPerformanceCounter(&s);
 //Простая формула
 return ((s.QuadPart)*10000/freq);
}

//Засекаем текущее время
void gunTimer::gunStartTiming()
{
	LARGE_INTEGER s;
	//Получаем счетчик
	QueryPerformanceCounter(&s);
	//Запоминаем время
	StartTime=s.QuadPart;
}

//Возвращает прошедшее время
UINT64 gunTimer::gunTimeElapsed()
{
	ElapsedTime=((gunTime()-StartTime)*10000/freq);
	return ElapsedTime;
}

//Преобразуем тики в секунды
double gunConvertTicksToSeconds(UINT64 ticks)
{
	return (double)(ticks/freq);
}
