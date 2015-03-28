/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#ifndef GUN_TYPES
#define GUN_TYPES

#include <windows.h>
#include <string>
#include <vector>

//Число пи
#define PI							3.14159265358979323846f
//Градусы -> радианы
#define DEGTORAD(a)					(((a) * PI) / 180.0f)
//Квадрат числа
#define SQR(x)						((x) * (x))
//Наименьшее
#define MAX(a,b)					((a < b) ? (b) : (a))
//Наибольшее
#define MIN(a,b)					((a < b) ? (a) : (b))


//Определим свои типы
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned __int64 UINT64;

//Структура цвета (3 байта)
struct gunColor3ub
{
	union {
		struct RGB
		{
			BYTE r,g,b;
		} RGBColor;
		BYTE Color[3];
	};
	gunColor3ub()
	{
		RGBColor.r=0;
		RGBColor.g=0;
		RGBColor.b=0;
	}
	gunColor3ub(BYTE r,BYTE g,BYTE b)
	{
		RGBColor.r=r;
		RGBColor.g=g;
		RGBColor.b=b;
	}
	BOOL operator ==(const gunColor3ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return FALSE;
		if (RGBColor.g!=c.RGBColor.g) return FALSE;
		if (RGBColor.b!=c.RGBColor.b) return FALSE;
		return TRUE;

	}
	BOOL operator !=(const gunColor3ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return TRUE;
		if (RGBColor.g!=c.RGBColor.g) return TRUE;
		if (RGBColor.b!=c.RGBColor.b) return TRUE;
		return FALSE;

	}
};

//Структура цвета (4 байта)
struct gunColor4ub
{
	union {
		struct RGB
		{
			BYTE r,g,b,a;
		} RGBColor;
		BYTE Color[4];
	};
	gunColor4ub()
	{
		RGBColor.r=0;
		RGBColor.g=0;
		RGBColor.b=0;
		RGBColor.a=255;
	}

	gunColor4ub(BYTE r,BYTE g,BYTE b,BYTE a)
	{
		RGBColor.r=r;
		RGBColor.g=g;
		RGBColor.b=b;
		RGBColor.a=a;
	}
	BOOL operator ==(const gunColor4ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return FALSE;
		if (RGBColor.g!=c.RGBColor.g) return FALSE;
		if (RGBColor.b!=c.RGBColor.b) return FALSE;
		if (RGBColor.a!=c.RGBColor.a) return FALSE;
		return TRUE;

	}
	BOOL operator !=(const gunColor4ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return TRUE;
		if (RGBColor.g!=c.RGBColor.g) return TRUE;
		if (RGBColor.b!=c.RGBColor.b) return TRUE;
		if (RGBColor.a!=c.RGBColor.a) return TRUE;
		return FALSE;

	}
};

// Макрос для итератора
#define IT(a) std::vector<a>::iterator

// Определим тип строки свой 
typedef std::basic_string<char> gunString;

// Все используемые именные константы
enum gunEnum {
	GUN_NONE,									// Ничего
	GUN_TEXTURE_BORDER_REPEAT,					// На границах повторяется
	GUN_TEXTURE_BORDER_CLAMP,					// На границах не повторяется
	GUN_TEXTURE_COMBINE_REPLACE,				// Смешать - Заменить
	GUN_TEXTURE_COMBINE_ADD,					// Смешать - Добаваить
	GUN_TEXTURE_COMBINE_MODULATE,				// Смешать - Умножить
	GUN_TEXTURE_DIFFUSE,						// Основная текстура
	GUN_RENDERMODE_3D,							// Режим 3D
	GUN_RENDERMODE_2D,							// Режим 2D для вывода текста
	GUN_RENDERMODE_PARTICLE,					
	GUN_RENDERMODE_BILLBOARD,
	GUN_RENDERMODE_RESTORE_LAST,				// Востановить предыдущий
};

#endif