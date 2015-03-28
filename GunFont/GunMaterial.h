/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#ifndef GUN_MATERIAL_FILE
#define GUN_MATERIAL_FILE

#pragma once

#include "GunTexture.h"				// Работа с текстурами

// Класс матерьяла
class gunMaterial
{
public:
	gunMaterial(void);
	// Коструктор
	gunMaterial(
		gunString _Name,
		int _DiffuseMap
	);

	~gunMaterial(void);

	// Имя материала
	gunString Name;
	// Основная текстура
	// Номер в массиве текстур
	int DiffuseMapID;
	// Возращает основную текстуру
	gunTexture& DiffuseMap();
	// Загрузка текстуры из файла
	BOOL LoadTextureFromFile(gunEnum type,const gunString& FileName);
	// Создание пустой текстуры
	BOOL CreateEmptyTexture(gunEnum type,int width,int height, gunColor3ub &c);
};

// Глобальный массив материалов
extern std::vector<gunMaterial> Material;

// Пустой материал
extern gunMaterial DEFAULT_MATERIAL;

// Новый материал
UINT gunNewMaterial();

#endif