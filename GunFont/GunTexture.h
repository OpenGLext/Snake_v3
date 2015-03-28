/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#pragma once

#include "GunTypes.h"

//Класс текстуры
class gunTexture
{
public:
	gunTexture(void)
	{
		Filter=10;
		Width=0;
		Height=0;
		TextureID=-1;
		Bpp=0;
		Border_Type=GUN_TEXTURE_BORDER_CLAMP;
	}
	~gunTexture(void);
    //Пиксели текстуры
	std::vector<BYTE> Data;
	//Способ фильтрации текстуры
	BYTE Filter;
	//Бит на пиксель
	BYTE Bpp;
	//Имя текстуры
	gunString TextureName;
	//Ширина текстуры
	UINT Width;
	//Высота текстуры
	UINT Height;
	//Индекс текстуры в видео памяти
	UINT TextureID;
	//Тип границ
	gunEnum Border_Type;

	//Создаёт текстуру, заполненную цветом в 3 компоненты
	void CreateEmpty(int width,int height, gunColor3ub &c);
	//Создаёт текстуру, заполненную цветом в 4 компоненты
	void CreateEmpty(int width,int height, gunColor4ub &c);

	//Загружаем из файла данные в зависимости от расширения
	BOOL LoadFromFile(const gunString &FileName);
	//Загружаем TGA файл
	BOOL LoadTGAFile(const gunString &FileName);
	//Загружаем JPG файл
	BOOL LoadJPGFile(const gunString &FileName);
};

// Глобальный массив текстур
extern std::vector<gunTexture> Texture;

UINT gunNewTexture();

// Запись экрана в файл JPG
// Используется для создания ScreenShot'ов
BOOL gunSaveScreenToJPG();
// Проверка наличия текстуры в массиве
int gunCheckTexture(const gunString& FileName);
