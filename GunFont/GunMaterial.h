/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#ifndef GUN_MATERIAL_FILE
#define GUN_MATERIAL_FILE

#pragma once

#include "GunTexture.h"				// ������ � ����������

// ����� ���������
class gunMaterial
{
public:
	gunMaterial(void);
	// ����������
	gunMaterial(
		gunString _Name,
		int _DiffuseMap
	);

	~gunMaterial(void);

	// ��� ���������
	gunString Name;
	// �������� ��������
	// ����� � ������� �������
	int DiffuseMapID;
	// ��������� �������� ��������
	gunTexture& DiffuseMap();
	// �������� �������� �� �����
	BOOL LoadTextureFromFile(gunEnum type,const gunString& FileName);
	// �������� ������ ��������
	BOOL CreateEmptyTexture(gunEnum type,int width,int height, gunColor3ub &c);
};

// ���������� ������ ����������
extern std::vector<gunMaterial> Material;

// ������ ��������
extern gunMaterial DEFAULT_MATERIAL;

// ����� ��������
UINT gunNewMaterial();

#endif