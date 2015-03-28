/***********************************************
*                                              *
*    �� ������ ���� ��� NeHe OpenGL BaseCode   *
*            http://nehe.gamedev.net           *
*                     2001                     *
*                                              *
***********************************************/
/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
/********************
*                   *
*  GUNgine Header   *
*                   *
**********************************************************************************
*                                                                                *
*	��� ���������� ���������� ��������� �������:                                 *
*                                                                                *
*   void InitSettings();                                                         *
*       ������������� ���������� �� ��������� �� OpenGL                          *
*		                                                                         *
*	BOOL Initialize (GL_Window* window, Keys* keys);                             *
*		������������ ��� �������������                                           *
*		���������� TRUE ���� ������������� �������, FALSE ���� ���               *
*		'window' ��� �������� ������������ ��� �������� ������ � ����            *
*		'keys' ��� ��������� ��� �������� ������� �������                        *
*                                                                                *
*	void Deinitialize (void);                                                    *
*		������������ ��� ���������������                                         *
*                                                                                *
*	void Update (DWORD milliseconds);                                            *
*		������������ ���������� �����                                            *
*		'milliseconds' ��� ���������� ���������� ��������� � �������� ������     *
*		������� ������������ � GunTimer � ������� QueryPerfomanceCounter         *
*                                                                                *
*	void Draw (void);                                                            *
*		������� ��������� ���� �����                                             *
*                                                                                *
*********************************************************************************/

#ifndef GL_FRAMEWORK__INCLUDED
#define GL_FRAMEWORK__INCLUDED


#pragma once

#include <windows.h>								// ������������ ���� ��� Windows
#include "GunSettings.h"

class GunWindow										// ����� ��� �������� �������������
{
public:
	GunWindow(void);
	~GunWindow(void);
};



void TerminateApplication (GL_Window* window);		// ���������� ���������

void ToggleFullscreen (GL_Window* window);			// ������������ Fullscreen / Windowed �����

// ��� ������� ���������� ������� ������
BOOL Initialize (GL_Window* window, Keys* keys);	// �������������

void Deinitialize (void);							// ���������������

void Update (UINT64 &DeltaTime);					// ������ ����� � ����������� �� ���������� �������

void Draw (void);									// ��������� ���� �����

void InitSettings();								// ������������� ��������

#define WM_TOGGLEFULLSCREEN (WM_USER+1)				// ���������� ����� ��������� ��� ������������ �����������

// ��������� ������
BOOL ProcessKeys(
   HWND hWnd,
   UINT message,
   WPARAM wParam = 0,
   LPARAM lParam = 0 
);

#endif												// GL_FRAMEWORK__INCLUDED

