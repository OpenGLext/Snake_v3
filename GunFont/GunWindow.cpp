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

#include "GunWindow.h"													// ������������ ���� ��� GunWindow
#include "GunRender.h"													// ������������ ���� ��� GunRender
#include "GunTimer.h"

#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

GunWindow::GunWindow(void)
{
}

GunWindow::~GunWindow(void)
{
}

static BOOL g_isProgramLooping;											// ���������� ����� �������� ����, ��� ������������ FullScreen/Windowed
static BOOL g_createFullScreen;											// ���� TRUE, ����� Fullscreen

void TerminateApplication (GL_Window* window)							// ����������� ����������
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// �������� ��������� A WM_QUIT
	g_isProgramLooping = FALSE;											// ��������� ��������� �����
}

void ToggleFullscreen (GL_Window* window)								// ������������ Fullscreen/Windowed
{
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// �������� ���������A WM_TOGGLEFULLSCREEN 
}

void ReshapeGL (int width, int height)									// ����������� ��������� ����, ����� ���������� ��� ��������� �������
{
	MainRender.Reshape(width, height);
}

BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel)	// ������ ���������� ������
{
	DEVMODE dmScreenSettings;											// ����� ����������
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// ��������� ��� �� �������
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// ������ ��������� Devmode
	dmScreenSettings.dmPelsWidth		= width;						// ������ ������
	dmScreenSettings.dmPelsHeight		= height;						// ������ ������
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// ��� �� �������
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// ���� �� ������� ���������� ����� ������, �� ���������� False
	}
	return TRUE;														// �� ����������, ���������� True
}

BOOL CreateWindowGL (GL_Window* window)									// ���� ��� ������ ���� OpenGL ����
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// ������� ����� ����
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// ������� ���������� ����� ����

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// ������� ���������� ����

	if (window->init.isFullScreen == TRUE)								// �������� Fullscreen, ��������� ������� ����� �����
	{
		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			// ����� Fullscreen �� ��������.  ����������� � ������ �����
			MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// ��������� isFullscreen � False (������� �����)
		}
		else															// ����� (���� Fullscreen ����� �����������)
		{
			ShowCursor (FALSE);											// ��������� ������ ����
			windowStyle = WS_POPUP;										// ������������� WindowStyle � WS_POPUP (����������� ����)
			windowExtendedStyle |= WS_EX_TOPMOST;						// ������������� ���������� ����� ���� � WS_EX_TOPMOST
		}																// (Top ���� ��������� �� ������ �����)
	}
	else																// ���� Fullscreen ����� ������
	{
		// �������� ���� �� ����� ������
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	// ������� OpenGL ����
	window->hWnd = CreateWindowEx (windowExtendedStyle,					// ���������� �����
								   window->init.application->className,	// ��� ������
								   window->init.title,					// ��������� ����
								   windowStyle,							// ����� ����
								   0, 0,								// X,Y ������� ����
								   windowRect.right - windowRect.left,	// ������ ����
								   windowRect.bottom - windowRect.top,	// ������ ����
								   HWND_DESKTOP,						// �������� ���� ������� ����
								   0,									// ��� ����
								   window->init.application->hInstance, // ������ Window Instance
								   window);

	if (window->hWnd == 0)												// ���� ��������� �������?
	{
		return FALSE;													// ���� ���, �� ��������� False
	}

	window->hDC = GetDC (window->hWnd);									// ��������� �������� ���������� ��� ����� ����
	if (window->hDC == 0)												// �������� ���������� ����������?
	{
		// ���
		DestroyWindow (window->hWnd);									// ���������� ����
		window->hWnd = 0;												// �������� ���������� ����
		return FALSE;													// ���������� False
	}

	//������������� OpenGL, ���� ��� �� �������.
	if (!MainRender.SetupOpenGL(window)) return FALSE;

	ShowWindow (window->hWnd, SW_NORMAL);								// ������� ���� �������
	window->isVisible = TRUE;											// ��������� isVisible � True

	ReshapeGL (window->init.width, window->init.height);				// �������� ���� ����

	ZeroMemory (window->keys, sizeof (Keys));							// ������� ��� �������

	return TRUE;														// �������� ���� ������ �������
																		// ������������� ���������� � WM_CREATE
}

// ��������� ���������
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �������� �������� ����
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	switch (uMsg)														// ������������ ���������
	{
		case WM_SYSCOMMAND:												// ������������� ��������� �������
		{
			switch (wParam)												// ��������� ��������� ������
			{
				case SC_SCREENSAVE:										// ��������� ������ �������� �����������?
				case SC_MONITORPOWER:									// ������� �������� ���� � ������ �����?
				return 0;												// �� ���� ����� ����, ����� ������ �� ����������
			}
			break;														// �����
		}
		return 0;														// ������� �� �������

		case WM_CREATE:													// �������� ����
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// ��������� ��������� ���� � ���������
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														// ������� �� �������

		case WM_CLOSE:													// ��������� ����
			TerminateApplication(window);								// ������������� ����������
		return 0;														// ������� �� �������

		case WM_SIZE:													// ���������� ����� ������� ����
			switch (wParam)												// �������� �����
			{
				case SIZE_MINIMIZED:									// ���� ��������������?
					window->isVisible = FALSE;							// ������������� isVisible � False
				return 0;												// ������� �� �������

				case SIZE_MAXIMIZED:									// ���� ���������������?
					window->isVisible = TRUE;							// ������������� isVisible � True
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// ��������� ���� - LoWord=Width, HiWord=Height
				return 0;												// ������� �� �������

				case SIZE_RESTORED:										// ���� ���� ������������?
					window->isVisible = TRUE;							// ������������� isVisible � True
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// ��������� - LoWord=Width, HiWord=Height
				return 0;												// ������� �� �������
			}
		break;															// �������

		case WM_KEYDOWN:												// ��������� ������ ����������, ������ �������
			if ((wParam >= 0) && (wParam <= 255))						// ������� (wParam) � ���������� ���������?
			{
				window->keys->keyDown [wParam] = TRUE;					// ������������� �������� ������� (wParam) � True
				ProcessKeys(hWnd,uMsg,wParam,lParam);
				return 0;												// ������� �� �������
			}
		break;															// �����

		case WM_KEYUP:													// ������� ��������
			if ((wParam >= 0) && (wParam <= 255))						// ������� (wParam) � ���������� ���������?
			{
				window->keys->keyDown [wParam] = FALSE;					// ������������� �������� ������� (wParam) � False
				return 0;												// ������� �� �������
			}
		break;															// �������

		case WM_TOGGLEFULLSCREEN:										// ������ ����� �����: FullScreen ����� ���./����.
			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			PostMessage (hWnd, WM_QUIT, 0, 0);
		break;															// �������
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);					// ��������� ��������� �������� � DefWindowProc
}

BOOL RegisterWindowClass (Application* application)						// ������������ ����� ���� ��� ������ ����������
{																		// TRUE ������ �� ���������
	// ������������ ������� �����
	WNDCLASSEX windowClass;												// ����� ����
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// �������� ��� ������ �������
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// ������ ��������� windowClass
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// �������������� ���� ��� ����� ��������� � ��������� �������
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// ���������� WindowProc ���������
	windowClass.hInstance		= application->hInstance;				// ������������� Instance
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// ����� ����� ����
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// ��������� ��������� (������ ����)
	windowClass.lpszClassName	= application->className;				// ������������� ��� ������ ����������
	if (RegisterClassEx (&windowClass) == 0)							// ����������� �����������?
	{
		// ��������: ������ �� ������ ������� ���������� 
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// ���������� False (������)
	}
	return TRUE;														// ���������� True (�� ��)
}

// ����� ����� ��������� (WinMain)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application			application;									// ��������� ����������
	GL_Window			window;											// ��������� ����
	Keys				keys;											// ��������� ������
	BOOL				isMessagePumpActive;							// ��������� �������?
	MSG					msg;											// ��������� ������� ���������
	

	// �������������� ��������� ���������
	InitSettings();

	// ��������� ������ ����������
	application.className = "GUNgine";									// ��� ������ ����������
	application.hInstance = hInstance;									// Instance ����������


	// ��������� ����
	ZeroMemory (&window, sizeof (GL_Window));							// �������� ��� ������ �������
	window.keys					= &keys;								// ��������� ������
	window.init.application		= &application;							// ����������
	window.init.title			= (char *)MainSettings.TITLE.data();	// ��������� ����
	window.init.width			= MainSettings.SCREEN_WIDTH;			// ������ ����
	window.init.height			= MainSettings.SCREEN_HEIGHT;			// ������ ����
	window.init.bitsPerPixel	= MainSettings.BPP;						// ����� �� �������
	window.init.isFullScreen	= MainSettings.FULLSCREEN;				// ����������? (���� TRUE)

	ZeroMemory (&keys, sizeof (Keys));									// ������� ��������� ������

	// ������������ ����� ��� ������ ����
	if (RegisterWindowClass (&application) == FALSE)					// ���������� �����������?
	{
		// ������
		MessageBox (HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// ��������� ����������
	}

	g_isProgramLooping = TRUE;											// ����������� ���� ������������� � TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen ������������� �� ���������

	// ������������� �������� �� �������� (�������� � �.�.)
	gunInitTimeOperation();
    gunTimer myTimer; // ��� �������
	myTimer.gunStartTiming();				// �������� �������

	while (g_isProgramLooping)											// ���� ���� WM_QUIT �� ��������
	{
		// ������ ����
		window.init.isFullScreen = g_createFullScreen;					// ������������� ��������� ��������� �������� ���� � Fullscreen?
		if (CreateWindowGL (&window) == TRUE)							// ���� ��������� �������?
		{
			// � ��� �� ����� ���� � ��� ����������������� OpenGL
			if (Initialize (&window, &keys) == FALSE)					// ��������� ������������� ����
			{
				// ������
				TerminateApplication (&window);							// ��������� ����, ��� �������� �������� ����������
			}
			else														// ����� (�������� ��������� ���������)
			{	// ������������� ������ �������
				isMessagePumpActive = TRUE;								// ������������� isMessagePumpActive � TRUE
				while (isMessagePumpActive == TRUE)						// ���� ��������� ��������� �������
				{
					// ������� �������� ����, ��������� ���������
					if (PeekMessage (&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// ��������� �� ��������� WM_QUIT
						if (msg.message != WM_QUIT)						// ��� ��������� A WM_QUIT?
						{
							DispatchMessage (&msg);						// ���� ���, ���������� ���������
						}
						else											// ����� (���� ��������� WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// ������������� ��������� ���������
						}
					}
					else												// ���� ���� ���������
					{
						if (window.isVisible == FALSE)					// ���� ���� �� ������
						{
							WaitMessage ();								// ���� ��������������, ���� ���������
						}
						else											// ���� ���� ������
						{
							// ������ ��������
							// ������������� �������, ��������� ����� ��������� � myTimer.ElapsedTime
							myTimer.gunTimeElapsed();   
							Update (myTimer.ElapsedTime);	// ���������, ������� � �.�.
							myTimer.gunStartTiming();		// �������� �������
							Draw ();						// ������ �����
							// ����������� ����� �����
							MainRender.Frame_Num++;
							

							SwapBuffers (window.hDC);					// ������ ������ (������� ������������)
						}
					}
				}														// ���� ���� isMessagePumpActive == TRUE
			}															// ���� (Initialize (...

			// ���������� ����������� ������, ��������������� ������
			Deinitialize ();											// ������������ ������������� �������

			MainRender.DestroyWindowGL (&window);									// ���������� �������� ����
		}
		else															// ���� �������� ���� �����������
		{
			// ������ �������� ����
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// ��������� ����
		}
	}																	// ���� (isProgramLooping)

	UnregisterClass (application.className, application.hInstance);		// ������� ����� ����
	return 0;
}																		// ����� WinMain()
