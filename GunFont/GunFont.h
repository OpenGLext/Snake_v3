#ifndef GUN_FONT_FILE
#define GUN_FONT_FILE

#include "GunTypes.h"
#include "GunTexture.h"

#pragma once

// Класс шрифта
class gunFont
{
public:
	// Конструктор
	gunFont(void)
	{
		Width=
		Height=
		Spacing=1;
		Color=gunColor4ub(255,255,255,255);
		Spacing=0;
	}
	~gunFont(void);
	// Название шрифта
	gunString Name;
	// Ширина
	float Width;
	// Высота
	float Height;
	// Текстура
	UINT TextureID;
	// Прозрачность
	gunEnum BlendFunc;
	// Базовая
	UINT Base;
	// Растояние между буквами
	float Spacing;
	// Растояния до реальных букв
	BYTE FTrim[256][2];
	// Цвет
	gunColor4ub Color;
	// Загрузка теустуры и нахождение пробелов между буквами
	// Zero - это пороговое значение. Ниже это ноль, выше есть пиксель.
	BOOL LoadTextureAndCheckSpace(const gunString &FileName,const gunString &FontName,BYTE Zero);
};

// Глобальный массив шрифтов
extern std::vector<gunFont> Font;
// Новый шрифт
UINT gunNewFont();
// Выбор шрифта
BOOL gunSelectFont(const gunString &Name);

#endif