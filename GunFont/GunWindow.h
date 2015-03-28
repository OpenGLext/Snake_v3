/***********************************************
*                                              *
*    За основу взят код NeHe OpenGL BaseCode   *
*            http://nehe.gamedev.net           *
*                     2001                     *
*                                              *
***********************************************/
/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
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
*	Нам необходимо определить следующие функции:                                 *
*                                                                                *
*   void InitSettings();                                                         *
*       Инициализация переменных не зависящих от OpenGL                          *
*		                                                                         *
*	BOOL Initialize (GL_Window* window, Keys* keys);                             *
*		Производится вся инициализация                                           *
*		Возвращает TRUE если инициализация успешна, FALSE если нет               *
*		'window' это параметр используемый для хранения данных о окне            *
*		'keys' это структура для хранения нажатых клавишь                        *
*                                                                                *
*	void Deinitialize (void);                                                    *
*		Производится вся деинициализация                                         *
*                                                                                *
*	void Update (DWORD milliseconds);                                            *
*		Производится обновление сцены                                            *
*		'milliseconds' это количество милисекунд прошедших с прошлого вызова     *
*		Подсчет производится в GunTimer с помощью QueryPerfomanceCounter         *
*                                                                                *
*	void Draw (void);                                                            *
*		Функция отрисовки всей сцены                                             *
*                                                                                *
*********************************************************************************/

#ifndef GL_FRAMEWORK__INCLUDED
#define GL_FRAMEWORK__INCLUDED


#pragma once

#include <windows.h>								// Заголовочный файл для Windows
#include "GunSettings.h"

class GunWindow										// Класс для будущего использования
{
public:
	GunWindow(void);
	~GunWindow(void);
};



void TerminateApplication (GL_Window* window);		// Завершение программы

void ToggleFullscreen (GL_Window* window);			// Переключение Fullscreen / Windowed режим

// Эту функцию необходимо описать самому
BOOL Initialize (GL_Window* window, Keys* keys);	// Инициализация

void Deinitialize (void);							// Деинициализация

void Update (UINT64 &DeltaTime);					// Расчет сцены в зависимости от прошедшего времени

void Draw (void);									// Отрисовка всей сцены

void InitSettings();								// Инициализация настроек

#define WM_TOGGLEFULLSCREEN (WM_USER+1)				// Определяем номер сообщения для переключения полноэкрана

// Обработка клавиш
BOOL ProcessKeys(
   HWND hWnd,
   UINT message,
   WPARAM wParam = 0,
   LPARAM lParam = 0 
);

#endif												// GL_FRAMEWORK__INCLUDED

