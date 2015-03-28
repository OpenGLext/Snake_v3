/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#include <windows.h>													// Заголовочный файл для Windows
#include "gunrender.h"

#include <gl/gl.h>														// Заголовочный файл для OpenGL32 Library
#include <gl/glu.h>														// Заголовочный файл для GLu32 Library

//Подключаем библиотеки, через ж... конечно, но зато не надо шариться в настройках проекта
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

//Глобальная переменная
gunRender MainRender;

gunRender::gunRender(void)
{
	Frame_Num=0;
	ISfps=FALSE;
	CurrentRenderMode=GUN_NONE;
}

gunRender::~gunRender(void)
{
}

// Для печати на экран
char buf[1000];

//Устанавливаем OpenGL
BOOL gunRender::SetupOpenGL(GL_Window *window)
{
	PIXELFORMATDESCRIPTOR pfd =											// pfd указывает Windows как устанавливать OpenGL
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Размер этого Pixel Format Descriptor'а
		1,																// Номер версии
		PFD_DRAW_TO_WINDOW |											// Формат должен поддерживать отрисовку в окно
		PFD_SUPPORT_OPENGL |											// Формат должен поддерживать OpenGL
		PFD_DOUBLEBUFFER,												// Формат должен поддерживать Double Buffering
		PFD_TYPE_RGBA,													// Формат должен поддерживать RGBA режим
		window->init.bitsPerPixel,										// Выбираем глубину цвета
		0, 0, 0, 0, 0, 0,												// Игнорируем биты цвета
		0,																// Нет альфа буфера (пока нету)
		0,																// Игнорируем бит сдвига
		0,																// Нету Accumulation Buffer'а
		0, 0, 0, 0,														// Accumulation биты игнорируем
		MainSettings.DEPTH,												// Глубина Z-Buffer'а (Depth Buffer)  
		0,																// Нету Stencil Buffer'а (пока нету)
		0,																// Нету Auxiliary Buffer'а
		PFD_MAIN_PLANE,													// Главный слой для отрисовки
		0,																// Зарезирвировано
		0, 0, 0															// Игнорируем маску слоёв
	};

	GLuint PixelFormat;													// Содержит выбранный формат

		PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// Ищем совместимый Pixel Format
	if (PixelFormat == 0)												// Мы нашли?
	{
		// Неудача
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// Пытаемся установить этот формат
	{
		// Неудачно
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	window->hRC = wglCreateContext (window->hDC);						// Пробуем получить контекст рендера
	if (window->hRC == 0)												// Мы его получили?
	{
		// Неудачно
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	// Делаем этот контект рендера текущим
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		// Неудачно
		wglDeleteContext (window->hRC);									// Удаляем контекст рендера
		window->hRC = 0;												// Обнуляем его
		ReleaseDC (window->hWnd, window->hDC);							// Освобождаем контекст устройсва
		window->hDC = 0;												// Обнуляем его
		DestroyWindow (window->hWnd);									// Убиваем окно
		window->hWnd = 0;												// Обнуляем хэндл окна
		return FALSE;													// Возвращаем False (Ложь)
	}

	return TRUE;
}

//Уничтожение OpenGL окна
BOOL gunRender::DestroyWindowGL (GL_Window* window)								// Уничтожаем OpenGL окно и освобождаем ресурсы
{
	if (window->hWnd != 0)												// Окно имеет ресурсы?
	{	
		if (window->hDC != 0)											// Окно имеет контекст устройства?
		{
			wglMakeCurrent (window->hDC, 0);							// Ставим текущий активны рендер контекст в ноль
			if (window->hRC != 0)										// Окно имеет рендер контекст?
			{
				wglDeleteContext (window->hRC);							// Осбобождаем рендер контекст
				window->hRC = 0;										// Обнуляем рендер контекст

			}
			ReleaseDC (window->hWnd, window->hDC);						// Освобождаем контекст устойства
			window->hDC = 0;											// Обнуляем контекст устойства
		}
		DestroyWindow (window->hWnd);									// Уничтожаем окно
		window->hWnd = 0;												// Обнуляем дискриптор окна
	}

	if (window->init.isFullScreen)										// Полноэкран?
	{
		ChangeDisplaySettings (NULL,0);									// Востанавливаем разрешение экрана
		ShowCursor (TRUE);												// Включаем курсор мыши
	}	
	return TRUE;														// Возращаем True
}

void gunRender::Reshape(int width, int height)
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Переустанавливаем ViewPort (область видимости)
	glMatrixMode (GL_PROJECTION);										// Выбираем матрицу проекции
	glLoadIdentity ();													// Сбрасываем её на единичную
	gluPerspective (MainSettings.FOV*(GLfloat)(height)/(GLfloat)(width), (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
		MainSettings.ZNEAR, MainSettings.ZFAR);		
	glMatrixMode (GL_MODELVIEW);										// Выбираем видовую матрицу
	glLoadIdentity ();													// Сбрасываем её на единичную


}

// Закачиваем текстуру в видео-память
void gunRender::TextureUpload(gunTexture &tex)
{
	// Строим текстуру из данных
	GLuint type,components;
	// В зависимости от бит на пиксель
	switch (tex.Bpp)
	{
		case 24: // 24 бита
			type=GL_RGB;components=3; break;
		case 32: // 32 бита
			type=GL_RGBA;components=4; break;
	};

	glGenTextures(1, (GLuint *)&tex.TextureID);			// Генерируем индекс OpenGL текстуры
	glBindTexture(GL_TEXTURE_2D, tex.TextureID);		// Биндим индекс на текстуру

	GLuint kak; // Тип границ текстуры
	// Выбираем тип границ
	switch (tex.Border_Type)
	{
		case GUN_TEXTURE_BORDER_CLAMP:	// Текстура не повторяемся
			kak=GL_CLAMP;break;
		case GUN_TEXTURE_BORDER_REPEAT: // Текстура повторяется 
			kak=GL_REPEAT;break;
	}

	// Устанавливаем параметры границ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kak);

	// Устанавливаем фильтрацию текстуры
	// 10 - самая лучшая, но медленная
	//  0 - самая плохая, но быстрая
	// ---------------------------------
	// Заметки: 10-й фильтр не самый лучший и в дальнейшем фильтры будут добавляться
	switch (tex.Filter)
	{
	case 10:
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 9 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 8 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 7 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 6 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 5 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 4 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 3 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 2 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 1 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 0 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}

	}

}

// Закачиваем текстуру в видео-память
void gunRender::TextureUpload(UINT texid)
{
	// Строим текстуру из данных
	GLuint type,components;
	// В зависимости от бит на пиксель
	switch (Texture[texid].Bpp)
	{
		case 24: // 24 бита
			type=GL_RGB;components=3; break;
		case 32: // 32 бита
			type=GL_RGBA;components=4; break;
	};

	glGenTextures(1, (GLuint *)&Texture[texid].TextureID);			// Генерируем индекс OpenGL текстуры
	glBindTexture(GL_TEXTURE_2D, Texture[texid].TextureID);		// Биндим индекс на текстуру

	GLuint kak; // Тип границ текстуры
	// Выбираем тип границ
	switch (Texture[texid].Border_Type)
	{
		case GUN_TEXTURE_BORDER_CLAMP:	// Текстура не повторяемся
			kak=GL_CLAMP;break;
		case GUN_TEXTURE_BORDER_REPEAT: // Текстура повторяется 
			kak=GL_REPEAT;break;
	}

	// Устанавливаем параметры границ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kak);

	// Устанавливаем фильтрацию текстуры
	// 10 - самая лучшая, но медленная
	//  0 - самая плохая, но быстрая
	// ---------------------------------
	// Заметки: 10-й фильтр не самый лучший и в дальнейшем фильтры будут добавляться
	switch (Texture[texid].Filter)
	{
	case 10:
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 9 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 8 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 7 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 6 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 5 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 4 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 3 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 2 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 1 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 0 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}

	}

}

// Включает текстуру
void gunRender::TextureEnable(gunTexture &tex)
{
	glBindTexture(GL_TEXTURE_2D, tex.TextureID);
}

// Включает текстуру
void gunRender::TextureEnable(UINT texid)
{
	glBindTexture(GL_TEXTURE_2D, Texture[texid].TextureID);
}

// Выключает текстуру
void gunRender::TextureDisable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Способ смешивания текстуры
void gunRender::TextureOperation(gunEnum op)
{
	// Операция смешавания:
	// Replace		- замена
	// Add			- добавление
	// Modulate		- умножение
	// Interpolate	- Интерполирования
	// ......
	// Постепенно будут добавляться
	switch (op)
	{
	case GUN_TEXTURE_COMBINE_MODULATE:
		{
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;
		}
	}

}


gunGLRender::gunGLRender(void)
{
}

gunGLRender::~gunGLRender(void)
{
}

// Сбрасывает экран в буффер
void gunGetScreen(BYTE *Data)
{
	// Выбираем буффер экрана
	glReadBuffer(GL_FRONT);
	// Под Data уже выделена память.
	// Читаем в неё весь экран
	glReadPixels(0,0,MainSettings.SCREEN_WIDTH,MainSettings.SCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,Data);
}

// Создаём шрифт
BOOL gunRender::CreateFont(gunFont &Font)
{
	float	cx;											// Содержит X координату буквы
	float	cy;											// Содержит Y координату буквы
    float   cwx;                                        // Ширина буквы в пикселях
	float   cwy;                                        // Высота буквы в пикселях

	// Вычисляем ширину и высоту
	// По умолчанию квадрат: 16х16
	cwx         = (1.0f / 256.0f) * 16;
	cwy         = (1.0f / 256.0f) * 16;

	Font.Base=glGenLists(256);								// Создаём 256 дисплей листов
	for (int loop=0; loop<256; loop++)						// Циклимся по всем листам
	{
		// X и Y позиция
		cx=float(loop%16)/16.0f;
		cy=float(loop/16)/16.0f;
		glNewList(Font.Base+loop,GL_COMPILE);				// Начинаем заполнять лист 
			glBegin(GL_QUADS);							// Рисуем букву четырехугольником
				glTexCoord2f(cx+(Font.FTrim[loop][0])/256.0f,cy+cwy);			
				glVertex2i(0,16);
				glTexCoord2f(cx+cwx-(16-Font.FTrim[loop][0]-Font.FTrim[loop][1])/256.0f,cy+cwy);
				glVertex2i(Font.FTrim[loop][1],16);
				glTexCoord2f(cx+cwx-(16-Font.FTrim[loop][0]-Font.FTrim[loop][1])/256.0f,cy);
				glVertex2i(Font.FTrim[loop][1],0);
				glTexCoord2f(cx+(Font.FTrim[loop][0])/256.0f,cy);
				glVertex2i(0,0);
			glEnd();									// Закончили рисовать
			// Перемещаемся на другую позицию
			glTranslated(Font.FTrim[loop][0]+Font.FTrim[loop][1]+Font.Spacing,0,0);
			
		glEndList();									// Закончили лист
	}													// Цикл по буквам
	
	return TRUE;
}

// Убиваем шрифт
void gunRender::KillFont(gunFont &Font)
{
	// Убиваем все буквы (листы)
	glDeleteLists(Font.Base, 256);
}

//Функция вывода текста в двухмерном режиме
// x,y - позиция
// Text - по аналогии с printf
void gunPrint2D(float x,float y,const char *Text,...)
{
	va_list		va;									// Ссылка на список аргументов
	if(Text=="")									// Если нету текста
		return;										// Ничего не делаем

	// Копируем текст в buf, заменяя переменные их значениями
	va_start(va, Text);
	vsprintf(buf, Text, va);
	va_end(va);

	// Устанавливаем цвет
	glColor4ubv(&Font[MainRender.CurFont].Color.Color[0]);
	// Включаем текстуру
	MainRender.TextureEnable(Font[MainRender.CurFont].TextureID);
	// Проверяем наличие 2D режима
	MainRender.RenderMode(GUN_RENDERMODE_2D);
	// Сохраняем видовую матрицу
	glPushMatrix();
	// Делаем её едениной
	glLoadIdentity();
	// Увеличиваем, в зависимости от ширины и высоты букв
	glScalef(Font[MainRender.CurFont].Width,Font[MainRender.CurFont].Height, 1);
	// Перемещаем на x и y
	glTranslated(x/Font[MainRender.CurFont].Width,y/Font[MainRender.CurFont].Height,0);
	// Смещаем базу листов на текущую
	glListBase(Font[MainRender.CurFont].Base);
	// Рисуем весь текст вызовом соответствующих листов
	glCallLists((GLsizei)strlen(buf), GL_UNSIGNED_BYTE, buf);
	// Востанавливаем видовую матрицу
	glPopMatrix();


}

// Установка режима
// 1. GUN_RENDERMODE_3D - Техмерный режим, обычное состояние
// 2. GUN_RENDERMODE_2D - Двухмерный режим, текст, спрайты и т.д.
// 3. GUN_RENDERMODE_RESTORE_LAST - Востанавливает предыдущий режим
void gunRender::RenderMode(gunEnum mode)
{
	switch (mode)
	{
	case GUN_RENDERMODE_3D:
		// Проверяем текущий режим
		if (CurrentRenderMode!=GUN_RENDERMODE_3D)
		{
			// Востановить предыдущий
			RenderMode(GUN_RENDERMODE_RESTORE_LAST);
			// Если был другой, то сменить
			CurrentRenderMode=GUN_RENDERMODE_3D;
		}
		break;
	case GUN_RENDERMODE_2D:
		// Проверяем текущий режим
		if (CurrentRenderMode!=GUN_RENDERMODE_2D)
		{
			// Востановить предыдущий
			RenderMode(GUN_RENDERMODE_RESTORE_LAST);
			// Сохраняем атрибуты
			glPushAttrib(GL_BLEND | GL_DRAW_BUFFER | GL_COLOR_BUFFER_BIT |
				GL_LOGIC_OP | GL_DEPTH_BUFFER_BIT |
				GL_DEPTH_TEST | GL_ENABLE_BIT | GL_LIGHTING_BIT | 
				GL_STENCIL_BUFFER_BIT );
			// Сохраняем матрицу проекции
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			// Делаем её единичной
			glLoadIdentity();
			// Устанвливаем 2D режим, в зависимости от заданных размеров
			glOrtho(0,FontDisplayWidth,FontDisplayWidth,0,-100,100);
			// Сохраняем видовую матрицу
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			// Делаем её еденичной
			glLoadIdentity();
			// Отключаем тест глубины
			glDisable(GL_DEPTH_TEST);
			// Операция прозрачности
			glBlendFunc(GL_ONE,GL_ONE);
			// Запрещаем запись в  буффер глубины
			glDepthMask(FALSE);
			// Устанавливаем текущий режим
			CurrentRenderMode=GUN_RENDERMODE_2D;
		}
		break;
	case GUN_RENDERMODE_RESTORE_LAST:
		// Если режим 2D
		if (CurrentRenderMode==GUN_RENDERMODE_2D)
		{
			// Востанавливаем матрицу проекции
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			// Востанавливаем видовую матрицу
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			// Востанавливаем атрибуты
			glPopAttrib();
		}
		// Текущий режим: Пусто
		CurrentRenderMode=GUN_NONE;
		break;
	}
}

// Инициализация вычисления fps (кадров в секунду)
// Time - время в секундах, интервал между вычислением fps
void gunRender::StartCalculateFPS(float Time)
{
	// fps вычисляется
	ISfps=TRUE;
	fps=0;
	// Засекаем номер кадра
	LastFPSFrame=Frame_Num;
	// Стартуем счетчик
	FPSTimer.gunStartTiming();
	// Интервал в тиках между вычислениями
	FPSTime=(UINT64)Time*10000;
}

//Обновление fps
void gunRender::UpdateFPS(UINT64 &DeltaTime)
{
	// Если fps не вычисляется, то выходим
	if (!ISfps) return;
	// Смотрим прошедшее время
	FPSTimer.gunTimeElapsed();
	// Если прошедшее время больше интервала
	if (FPSTimer.ElapsedTime>FPSTime)
	{
		// Вычисляем fps
		// Колличество_кадров*10000/Прошедшее_время
		fps=(UINT)((Frame_Num-LastFPSFrame)*10000.f/FPSTimer.ElapsedTime);
		// Стартуем счетчик
		FPSTimer.gunStartTiming();
		// Запоминаем кадр
		LastFPSFrame=Frame_Num;
	}
}