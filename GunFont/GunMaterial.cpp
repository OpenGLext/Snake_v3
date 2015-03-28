/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#include "gunmaterial.h"

// ���������� ������ ����������
std::vector<gunMaterial> Material;

// ������� ��������
gunMaterial DEFAULT_MATERIAL(
	"Default Material",
	gunNewTexture()
	);

gunMaterial::gunMaterial(void)
{
}

// �����������
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

// ���������� ������ �� �������� ��������
gunTexture& gunMaterial::DiffuseMap()
{
	return Texture[DiffuseMapID];
}

// �������� ������ ���������
UINT gunNewMaterial()
{
	Material.push_back(DEFAULT_MATERIAL);
	return (UINT)Material.size()-1;
}

// �������� ������ ��������, ���������� ������, � ������� ��������
// 'type' ����� ��������� ��������:
// 1. GUN_TEXTURE_DIFFUSE - �������� ��������
BOOL gunMaterial::CreateEmptyTexture(gunEnum type,int width,int height, gunColor3ub &c)
{
	switch (type)
	{
	case GUN_TEXTURE_DIFFUSE:
		// ������ �������� � ��������� ������
		DiffuseMapID=gunNewTexture();
		// ��������� ��������
		DiffuseMap().CreateEmpty(width,height,c);
		return TRUE;
		break;
	}
	return FALSE;
}

// �������� �������� �� ����� FileName.
// 'type' ����� ��������� ��������:
// 1. GUN_TEXTURE_DIFFUSE - �������� ��������
BOOL gunMaterial::LoadTextureFromFile(gunEnum type,const gunString& FileName)
{
	switch (type)
	{
	case GUN_TEXTURE_DIFFUSE:
		// ��������� �� ������� ����� ��������
		if (DiffuseMapID=gunCheckTexture(FileName)==-1)
		{
			// ���� ���, �� ������ �����
			DiffuseMapID=gunNewTexture();
			return DiffuseMap().LoadFromFile(FileName);
		}
		break;

	}
	return FALSE;
}