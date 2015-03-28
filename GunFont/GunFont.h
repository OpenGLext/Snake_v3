#ifndef GUN_FONT_FILE
#define GUN_FONT_FILE

#include "GunTypes.h"
#include "GunTexture.h"

#pragma once

// ����� ������
class gunFont
{
public:
	// �����������
	gunFont(void)
	{
		Width=
		Height=
		Spacing=1;
		Color=gunColor4ub(255,255,255,255);
		Spacing=0;
	}
	~gunFont(void);
	// �������� ������
	gunString Name;
	// ������
	float Width;
	// ������
	float Height;
	// ��������
	UINT TextureID;
	// ������������
	gunEnum BlendFunc;
	// �������
	UINT Base;
	// ��������� ����� �������
	float Spacing;
	// ��������� �� �������� ����
	BYTE FTrim[256][2];
	// ����
	gunColor4ub Color;
	// �������� �������� � ���������� �������� ����� �������
	// Zero - ��� ��������� ��������. ���� ��� ����, ���� ���� �������.
	BOOL LoadTextureAndCheckSpace(const gunString &FileName,const gunString &FontName,BYTE Zero);
};

// ���������� ������ �������
extern std::vector<gunFont> Font;
// ����� �����
UINT gunNewFont();
// ����� ������
BOOL gunSelectFont(const gunString &Name);

#endif