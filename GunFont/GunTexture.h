/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#pragma once

#include "GunTypes.h"

//����� ��������
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
    //������� ��������
	std::vector<BYTE> Data;
	//������ ���������� ��������
	BYTE Filter;
	//��� �� �������
	BYTE Bpp;
	//��� ��������
	gunString TextureName;
	//������ ��������
	UINT Width;
	//������ ��������
	UINT Height;
	//������ �������� � ����� ������
	UINT TextureID;
	//��� ������
	gunEnum Border_Type;

	//������ ��������, ����������� ������ � 3 ����������
	void CreateEmpty(int width,int height, gunColor3ub &c);
	//������ ��������, ����������� ������ � 4 ����������
	void CreateEmpty(int width,int height, gunColor4ub &c);

	//��������� �� ����� ������ � ����������� �� ����������
	BOOL LoadFromFile(const gunString &FileName);
	//��������� TGA ����
	BOOL LoadTGAFile(const gunString &FileName);
	//��������� JPG ����
	BOOL LoadJPGFile(const gunString &FileName);
};

// ���������� ������ �������
extern std::vector<gunTexture> Texture;

UINT gunNewTexture();

// ������ ������ � ���� JPG
// ������������ ��� �������� ScreenShot'��
BOOL gunSaveScreenToJPG();
// �������� ������� �������� � �������
int gunCheckTexture(const gunString& FileName);
