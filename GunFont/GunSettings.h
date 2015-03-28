/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#pragma once

#include "GunTypes.h"								// ����

typedef struct {									// ��������� ��� ����������
	BOOL keyDown [256];								// ������ TRUE / FALSE ��� ������ �������
} Keys;												// ��� Keys

typedef struct {									// �������� ���������� ������������� ��� ���������
	HINSTANCE		hInstance;						// Application Instance
	const char*		className;						// Application ClassName
} Application;										// Application

typedef struct {									// ���������� ��� �������� ����
	Application*		application;				// ��������� Application 
	char*				title;						// �������� ����
	int					width;						// ������
	int					height;						// ������
	int					bitsPerPixel;				// ��� �� �������
	BOOL				isFullScreen;				// �������������?
} GL_WindowInit;									// GL_WindowInit

typedef struct {									// �������� ���������� ������������� ��� Window
	Keys*				keys;						// ��������� ��� ��������
	HWND				hWnd;						// ����� ��� ���� 
	HDC					hDC;						// �������� ����������
	HGLRC				hRC;						// �������� OpenGL
	GL_WindowInit		init;						// ������������� ����
	BOOL				isVisible;					// ���� ������?
	DWORD				lastTickCount;				// ������� �����
} GL_Window;										// GL_Window

//����� ��������
class gunSettings
{
public:
	gunSettings(void);
	~gunSettings(void);

	//�������� ����
	gunString TITLE;
	//������ ������
	UINT SCREEN_WIDTH;
	//������ ������
	UINT SCREEN_HEIGHT;
	//�� ������ �����?
	BOOL FULLSCREEN;
	//������� ����� (��� �� �������)
	BYTE BPP;
	//�������� Z-�������
	BYTE DEPTH;
	// ���� ������
	// �� ��������� ���� ������ ��������� �� ���������,
	// �� ����, �������� � Quake, �������� � ��������������� ����.
	// � ��� ����� � �.
	float FOV;
	//������� ��������� ���������
	float ZNEAR;
	//������� ��������� ���������
	float ZFAR;
};

//���������� ����������
extern gunSettings MainSettings;