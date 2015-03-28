/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#ifndef GUN_TYPES
#define GUN_TYPES

#include <windows.h>
#include <string>
#include <vector>

//����� ��
#define PI							3.14159265358979323846f
//������� -> �������
#define DEGTORAD(a)					(((a) * PI) / 180.0f)
//������� �����
#define SQR(x)						((x) * (x))
//����������
#define MAX(a,b)					((a < b) ? (b) : (a))
//����������
#define MIN(a,b)					((a < b) ? (a) : (b))


//��������� ���� ����
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned __int64 UINT64;

//��������� ����� (3 �����)
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

//��������� ����� (4 �����)
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

// ������ ��� ���������
#define IT(a) std::vector<a>::iterator

// ��������� ��� ������ ���� 
typedef std::basic_string<char> gunString;

// ��� ������������ ������� ���������
enum gunEnum {
	GUN_NONE,									// ������
	GUN_TEXTURE_BORDER_REPEAT,					// �� �������� �����������
	GUN_TEXTURE_BORDER_CLAMP,					// �� �������� �� �����������
	GUN_TEXTURE_COMBINE_REPLACE,				// ������� - ��������
	GUN_TEXTURE_COMBINE_ADD,					// ������� - ���������
	GUN_TEXTURE_COMBINE_MODULATE,				// ������� - ��������
	GUN_TEXTURE_DIFFUSE,						// �������� ��������
	GUN_RENDERMODE_3D,							// ����� 3D
	GUN_RENDERMODE_2D,							// ����� 2D ��� ������ ������
	GUN_RENDERMODE_PARTICLE,					
	GUN_RENDERMODE_BILLBOARD,
	GUN_RENDERMODE_RESTORE_LAST,				// ����������� ����������
};

#endif