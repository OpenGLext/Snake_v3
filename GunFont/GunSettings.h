/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#pragma once

#include "GunTypes.h"								// Типы

typedef struct {									// Структура для клавиатуры
	BOOL keyDown [256];								// Хранит TRUE / FALSE для каждой клавиши
} Keys;												// Тип Keys

typedef struct {									// Содержит информацию жизненоважную для программы
	HINSTANCE		hInstance;						// Application Instance
	const char*		className;						// Application ClassName
} Application;										// Application

typedef struct {									// Информация для создания окна
	Application*		application;				// структура Application 
	char*				title;						// Название окна
	int					width;						// Ширина
	int					height;						// Высота
	int					bitsPerPixel;				// Бит на пиксель
	BOOL				isFullScreen;				// Полноэкранное?
} GL_WindowInit;									// GL_WindowInit

typedef struct {									// Содержит информацию жизненоважную для Window
	Keys*				keys;						// Структура для клавишей
	HWND				hWnd;						// Хэндл для окна 
	HDC					hDC;						// Контекст устройства
	HGLRC				hRC;						// Контекст OpenGL
	GL_WindowInit		init;						// Инициализация окна
	BOOL				isVisible;					// Окно видимо?
	DWORD				lastTickCount;				// Счетчик тиков
} GL_Window;										// GL_Window

//Класс настроек
class gunSettings
{
public:
	gunSettings(void);
	~gunSettings(void);

	//Название окна
	gunString TITLE;
	//Ширина экрана
	UINT SCREEN_WIDTH;
	//Высота экрана
	UINT SCREEN_HEIGHT;
	//На полный экран?
	BOOL FULLSCREEN;
	//Глубина цвета (бит на пиксель)
	BYTE BPP;
	//Битность Z-буффера
	BYTE DEPTH;
	// Угол обзора
	// По умолчанию угол обзора считается по вертикали,
	// но люди, играющие в Quake, привыкли к горизонтальному углу.
	// В том числе и я.
	float FOV;
	//Ближняя плоскость отсечения
	float ZNEAR;
	//Дальняя плоскость отсечения
	float ZFAR;
};

//Глобальная переменная
extern gunSettings MainSettings;