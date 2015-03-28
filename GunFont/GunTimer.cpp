/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
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

//������� ������ �������
UINT64 freq;


//�������������� ��� �������� �� ��������
void gunInitTimeOperation()
{
 LARGE_INTEGER s;
 //�������� �������
 QueryPerformanceFrequency(&s);
 //��������� �
 freq=s.QuadPart;
}
//������� �������� ����������� ��������
UINT64 gunTime()
{
 LARGE_INTEGER s;
 //�������� �������
 QueryPerformanceCounter(&s);
 //���������� ������ �������� �����
 return s.QuadPart;
}
  
//���������� ����� � �����
UINT64 gunGetTicksTime()
{
 LARGE_INTEGER s;
 //�������� �������
 QueryPerformanceCounter(&s);
 //������� �������
 return ((s.QuadPart)*10000/freq);
}

//�������� ������� �����
void gunTimer::gunStartTiming()
{
	LARGE_INTEGER s;
	//�������� �������
	QueryPerformanceCounter(&s);
	//���������� �����
	StartTime=s.QuadPart;
}

//���������� ��������� �����
UINT64 gunTimer::gunTimeElapsed()
{
	ElapsedTime=((gunTime()-StartTime)*10000/freq);
	return ElapsedTime;
}

//����������� ���� � �������
double gunConvertTicksToSeconds(UINT64 ticks)
{
	return (double)(ticks/freq);
}
