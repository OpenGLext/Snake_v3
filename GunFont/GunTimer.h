/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#pragma once
#include "GunTypes.h"

//����� �������
class gunTimer
{
public:
	gunTimer(void);
	~gunTimer(void);
	//��� �������� �������
	UINT64 StartTime;
	//��������� �����
	UINT64 ElapsedTime;
	//��������� �����
	void gunStartTiming();
	//������� �������
	UINT64 gunTimeElapsed();
};

//������� �������� ��������
UINT64 gunTime();
//���������� � �����
UINT64 gunGetTicksTime();
//������������ � ��������
double gunConvertTicksToSeconds(UINT64 ticks);
// �������������
void gunInitTimeOperation();