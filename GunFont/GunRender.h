/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#pragma once
#include "GunSettings.h"
#include "GunMaterial.h"
#include "GunFont.h"
#include "GunTimer.h"

//Класс основного рендера
class gunRender
{
public:
	gunRender(void);
	~gunRender(void);
	// Инициализация OpenGL
	BOOL SetupOpenGL(GL_Window *window);
	// Деинициализация OpenGL
	BOOL DestroyWindowGL (GL_Window* window);
	// Коррекция окна
	void Reshape(int width, int height);

	// Номер текущего кадра
	UINT64 Frame_Num;
	// Кадров в секунду
	UINT fps;
	// Таймер для fps
    gunTimer FPSTimer;
	// Считать fps?
	BOOL ISfps;
	// Время между обновлениями fps
	UINT64 FPSTime;
	// Последний кадр обновления fps
	UINT64 LastFPSFrame;
	// Начать считать fps, промежуток в секундах
	void StartCalculateFPS(float Time);
	// Обновить fps, время в тиках
	void UpdateFPS(UINT64 &DeltaTime);

	//Закачиваем в видео карточку данные по текстуре
	void TextureUpload(gunTexture &tex);
	//Закачиваем в видео карточку данные по индексу в массиве
	void TextureUpload(UINT texid);
	//Включаем текстуру по ссылке на текстуру
	void TextureEnable(gunTexture &tex);
	//Включаем текстуру по индексу в массиве
	void TextureEnable(UINT texid);
	//Выключаем текстуру
	void TextureDisable();
	//Операция для смешивания текстуры
	void TextureOperation(gunEnum op);

	// Режим отрисовки (3D, 2D для текста, бильборды)
	gunEnum CurrentRenderMode;
	void RenderMode(gunEnum mode);

	// Операции со шрифтами
	// Текущий шрифт
	UINT CurFont;
	// Ширина экрана для шрифта
	UINT FontDisplayWidth;
	// Высота экрана для шрифта
	UINT FontDisplayHeight;
	// Настройка двухмерного режима
	void TextMode2D(UINT Width, UINT Height)
	{
		FontDisplayWidth=Width;
		FontDisplayHeight=Height;
	}
	// Создание шрифта
	BOOL CreateFont(gunFont &Font);
	// Уничтожение шрифта
	void KillFont(gunFont &Font);
};

// Оставил место для поддержки разных API
// Если сделать основной класс с виртуальными функциями , и наследовать с переопределением,
// то возможно сделать разные рендеры.
class gunGLRender :
	public gunRender
{
public:
	gunGLRender(void);
	~gunGLRender(void);
};

//Глобальная переменная
extern gunRender MainRender;

// Функция сброса экрана в буффер
void gunGetScreen(BYTE *Data);

//Вывод текста
void gunPrint2D(float x,float y,const char *Text, ...);