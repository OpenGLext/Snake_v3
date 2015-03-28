/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#pragma once
#include "GunSettings.h"
#include "GunMaterial.h"
#include "GunFont.h"
#include "GunTimer.h"

//����� ��������� �������
class gunRender
{
public:
	gunRender(void);
	~gunRender(void);
	// ������������� OpenGL
	BOOL SetupOpenGL(GL_Window *window);
	// ��������������� OpenGL
	BOOL DestroyWindowGL (GL_Window* window);
	// ��������� ����
	void Reshape(int width, int height);

	// ����� �������� �����
	UINT64 Frame_Num;
	// ������ � �������
	UINT fps;
	// ������ ��� fps
    gunTimer FPSTimer;
	// ������� fps?
	BOOL ISfps;
	// ����� ����� ������������ fps
	UINT64 FPSTime;
	// ��������� ���� ���������� fps
	UINT64 LastFPSFrame;
	// ������ ������� fps, ���������� � ��������
	void StartCalculateFPS(float Time);
	// �������� fps, ����� � �����
	void UpdateFPS(UINT64 &DeltaTime);

	//���������� � ����� �������� ������ �� ��������
	void TextureUpload(gunTexture &tex);
	//���������� � ����� �������� ������ �� ������� � �������
	void TextureUpload(UINT texid);
	//�������� �������� �� ������ �� ��������
	void TextureEnable(gunTexture &tex);
	//�������� �������� �� ������� � �������
	void TextureEnable(UINT texid);
	//��������� ��������
	void TextureDisable();
	//�������� ��� ���������� ��������
	void TextureOperation(gunEnum op);

	// ����� ��������� (3D, 2D ��� ������, ���������)
	gunEnum CurrentRenderMode;
	void RenderMode(gunEnum mode);

	// �������� �� ��������
	// ������� �����
	UINT CurFont;
	// ������ ������ ��� ������
	UINT FontDisplayWidth;
	// ������ ������ ��� ������
	UINT FontDisplayHeight;
	// ��������� ����������� ������
	void TextMode2D(UINT Width, UINT Height)
	{
		FontDisplayWidth=Width;
		FontDisplayHeight=Height;
	}
	// �������� ������
	BOOL CreateFont(gunFont &Font);
	// ����������� ������
	void KillFont(gunFont &Font);
};

// ������� ����� ��� ��������� ������ API
// ���� ������� �������� ����� � ������������ ��������� , � ����������� � ����������������,
// �� �������� ������� ������ �������.
class gunGLRender :
	public gunRender
{
public:
	gunGLRender(void);
	~gunGLRender(void);
};

//���������� ����������
extern gunRender MainRender;

// ������� ������ ������ � ������
void gunGetScreen(BYTE *Data);

//����� ������
void gunPrint2D(float x,float y,const char *Text, ...);