/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#include "gunmaterial.h"

// Глобальный массив материалов
std::vector<gunMaterial> Material;

// Обычный материал
gunMaterial DEFAULT_MATERIAL(
	"Default Material",
	gunNewTexture()
	);

gunMaterial::gunMaterial(void)
{
}

// Конструктор
gunMaterial::gunMaterial(
		gunString _Name,
		int _DiffuseMap
	)
{
	Name=_Name;
	DiffuseMapID=_DiffuseMap;

}

gunMaterial::~gunMaterial(void)
{
}

// Возвращает ссылку на основную текстуру
gunTexture& gunMaterial::DiffuseMap()
{
	return Texture[DiffuseMapID];
}

// Создание нового материала
UINT gunNewMaterial()
{
	Material.push_back(DEFAULT_MATERIAL);
	return (UINT)Material.size()-1;
}

// Создание пустой текстуры, заполненой цветом, и заданым размером
// 'type' может принимать значения:
// 1. GUN_TEXTURE_DIFFUSE - основная текстура
BOOL gunMaterial::CreateEmptyTexture(gunEnum type,int width,int height, gunColor3ub &c)
{
	switch (type)
	{
	case GUN_TEXTURE_DIFFUSE:
		// Создаём текстуру и сохраняет индекс
		DiffuseMapID=gunNewTexture();
		// Заполняем текстуру
		DiffuseMap().CreateEmpty(width,height,c);
		return TRUE;
		break;
	}
	return FALSE;
}

// Создание текстуры из файла FileName.
// 'type' может принимать значения:
// 1. GUN_TEXTURE_DIFFUSE - основная текстура
BOOL gunMaterial::LoadTextureFromFile(gunEnum type,const gunString& FileName)
{
	switch (type)
	{
	case GUN_TEXTURE_DIFFUSE:
		// Проверяем на наличие такой текстуры
		if (DiffuseMapID=gunCheckTexture(FileName)==-1)
		{
			// Если нет, то создаём новую
			DiffuseMapID=gunNewTexture();
			return DiffuseMap().LoadFromFile(FileName);
		}
		break;

	}
	return FALSE;
}