/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#include <windows.h>													// ������������ ���� ��� Windows
#include "gunrender.h"

#include <gl/gl.h>														// ������������ ���� ��� OpenGL32 Library
#include <gl/glu.h>														// ������������ ���� ��� GLu32 Library

//���������� ����������, ����� �... �������, �� ���� �� ���� �������� � ���������� �������
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

//���������� ����������
gunRender MainRender;

gunRender::gunRender(void)
{
	Frame_Num=0;
	ISfps=FALSE;
	CurrentRenderMode=GUN_NONE;
}

gunRender::~gunRender(void)
{
}

// ��� ������ �� �����
char buf[1000];

//������������� OpenGL
BOOL gunRender::SetupOpenGL(GL_Window *window)
{
	PIXELFORMATDESCRIPTOR pfd =											// pfd ��������� Windows ��� ������������� OpenGL
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// ������ ����� Pixel Format Descriptor'�
		1,																// ����� ������
		PFD_DRAW_TO_WINDOW |											// ������ ������ ������������ ��������� � ����
		PFD_SUPPORT_OPENGL |											// ������ ������ ������������ OpenGL
		PFD_DOUBLEBUFFER,												// ������ ������ ������������ Double Buffering
		PFD_TYPE_RGBA,													// ������ ������ ������������ RGBA �����
		window->init.bitsPerPixel,										// �������� ������� �����
		0, 0, 0, 0, 0, 0,												// ���������� ���� �����
		0,																// ��� ����� ������ (���� ����)
		0,																// ���������� ��� ������
		0,																// ���� Accumulation Buffer'�
		0, 0, 0, 0,														// Accumulation ���� ����������
		MainSettings.DEPTH,												// ������� Z-Buffer'� (Depth Buffer)  
		0,																// ���� Stencil Buffer'� (���� ����)
		0,																// ���� Auxiliary Buffer'�
		PFD_MAIN_PLANE,													// ������� ���� ��� ���������
		0,																// ���������������
		0, 0, 0															// ���������� ����� ����
	};

	GLuint PixelFormat;													// �������� ��������� ������

		PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// ���� ����������� Pixel Format
	if (PixelFormat == 0)												// �� �����?
	{
		// �������
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// �������� ���������� ���� ������
	{
		// ��������
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	window->hRC = wglCreateContext (window->hDC);						// ������� �������� �������� �������
	if (window->hRC == 0)												// �� ��� ��������?
	{
		// ��������
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	// ������ ���� ������� ������� �������
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		// ��������
		wglDeleteContext (window->hRC);									// ������� �������� �������
		window->hRC = 0;												// �������� ���
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	return TRUE;
}

//����������� OpenGL ����
BOOL gunRender::DestroyWindowGL (GL_Window* window)								// ���������� OpenGL ���� � ����������� �������
{
	if (window->hWnd != 0)												// ���� ����� �������?
	{	
		if (window->hDC != 0)											// ���� ����� �������� ����������?
		{
			wglMakeCurrent (window->hDC, 0);							// ������ ������� ������� ������ �������� � ����
			if (window->hRC != 0)										// ���� ����� ������ ��������?
			{
				wglDeleteContext (window->hRC);							// ����������� ������ ��������
				window->hRC = 0;										// �������� ������ ��������

			}
			ReleaseDC (window->hWnd, window->hDC);						// ����������� �������� ���������
			window->hDC = 0;											// �������� �������� ���������
		}
		DestroyWindow (window->hWnd);									// ���������� ����
		window->hWnd = 0;												// �������� ���������� ����
	}

	if (window->init.isFullScreen)										// ����������?
	{
		ChangeDisplaySettings (NULL,0);									// �������������� ���������� ������
		ShowCursor (TRUE);												// �������� ������ ����
	}	
	return TRUE;														// ��������� True
}

void gunRender::Reshape(int width, int height)
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// ����������������� ViewPort (������� ���������)
	glMatrixMode (GL_PROJECTION);										// �������� ������� ��������
	glLoadIdentity ();													// ���������� � �� ���������
	gluPerspective (MainSettings.FOV*(GLfloat)(height)/(GLfloat)(width), (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
		MainSettings.ZNEAR, MainSettings.ZFAR);		
	glMatrixMode (GL_MODELVIEW);										// �������� ������� �������
	glLoadIdentity ();													// ���������� � �� ���������


}

// ���������� �������� � �����-������
void gunRender::TextureUpload(gunTexture &tex)
{
	// ������ �������� �� ������
	GLuint type,components;
	// � ����������� �� ��� �� �������
	switch (tex.Bpp)
	{
		case 24: // 24 ����
			type=GL_RGB;components=3; break;
		case 32: // 32 ����
			type=GL_RGBA;components=4; break;
	};

	glGenTextures(1, (GLuint *)&tex.TextureID);			// ���������� ������ OpenGL ��������
	glBindTexture(GL_TEXTURE_2D, tex.TextureID);		// ������ ������ �� ��������

	GLuint kak; // ��� ������ ��������
	// �������� ��� ������
	switch (tex.Border_Type)
	{
		case GUN_TEXTURE_BORDER_CLAMP:	// �������� �� �����������
			kak=GL_CLAMP;break;
		case GUN_TEXTURE_BORDER_REPEAT: // �������� ����������� 
			kak=GL_REPEAT;break;
	}

	// ������������� ��������� ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kak);

	// ������������� ���������� ��������
	// 10 - ����� ������, �� ���������
	//  0 - ����� ������, �� �������
	// ---------------------------------
	// �������: 10-� ������ �� ����� ������ � � ���������� ������� ����� �����������
	switch (tex.Filter)
	{
	case 10:
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 9 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 8 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 7 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 6 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 5 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 4 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex.Width, tex.Height, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 3 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 2 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 1 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}
	case 0 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, tex.Width, tex.Height, 0, type, GL_UNSIGNED_BYTE, &tex.Data[0]);
			break;
		}

	}

}

// ���������� �������� � �����-������
void gunRender::TextureUpload(UINT texid)
{
	// ������ �������� �� ������
	GLuint type,components;
	// � ����������� �� ��� �� �������
	switch (Texture[texid].Bpp)
	{
		case 24: // 24 ����
			type=GL_RGB;components=3; break;
		case 32: // 32 ����
			type=GL_RGBA;components=4; break;
	};

	glGenTextures(1, (GLuint *)&Texture[texid].TextureID);			// ���������� ������ OpenGL ��������
	glBindTexture(GL_TEXTURE_2D, Texture[texid].TextureID);		// ������ ������ �� ��������

	GLuint kak; // ��� ������ ��������
	// �������� ��� ������
	switch (Texture[texid].Border_Type)
	{
		case GUN_TEXTURE_BORDER_CLAMP:	// �������� �� �����������
			kak=GL_CLAMP;break;
		case GUN_TEXTURE_BORDER_REPEAT: // �������� ����������� 
			kak=GL_REPEAT;break;
	}

	// ������������� ��������� ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kak);

	// ������������� ���������� ��������
	// 10 - ����� ������, �� ���������
	//  0 - ����� ������, �� �������
	// ---------------------------------
	// �������: 10-� ������ �� ����� ������ � � ���������� ������� ����� �����������
	switch (Texture[texid].Filter)
	{
	case 10:
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 9 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 8 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 7 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 6 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 5 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 4 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, components, Texture[texid].Width, Texture[texid].Height, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 3 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 2 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 1 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}
	case 0 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
			glTexImage2D(GL_TEXTURE_2D, 0, type, Texture[texid].Width, Texture[texid].Height, 0, type, GL_UNSIGNED_BYTE, &Texture[texid].Data[0]);
			break;
		}

	}

}

// �������� ��������
void gunRender::TextureEnable(gunTexture &tex)
{
	glBindTexture(GL_TEXTURE_2D, tex.TextureID);
}

// �������� ��������
void gunRender::TextureEnable(UINT texid)
{
	glBindTexture(GL_TEXTURE_2D, Texture[texid].TextureID);
}

// ��������� ��������
void gunRender::TextureDisable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

// ������ ���������� ��������
void gunRender::TextureOperation(gunEnum op)
{
	// �������� ����������:
	// Replace		- ������
	// Add			- ����������
	// Modulate		- ���������
	// Interpolate	- ����������������
	// ......
	// ���������� ����� �����������
	switch (op)
	{
	case GUN_TEXTURE_COMBINE_MODULATE:
		{
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;
		}
	}

}


gunGLRender::gunGLRender(void)
{
}

gunGLRender::~gunGLRender(void)
{
}

// ���������� ����� � ������
void gunGetScreen(BYTE *Data)
{
	// �������� ������ ������
	glReadBuffer(GL_FRONT);
	// ��� Data ��� �������� ������.
	// ������ � �� ���� �����
	glReadPixels(0,0,MainSettings.SCREEN_WIDTH,MainSettings.SCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,Data);
}

// ������ �����
BOOL gunRender::CreateFont(gunFont &Font)
{
	float	cx;											// �������� X ���������� �����
	float	cy;											// �������� Y ���������� �����
    float   cwx;                                        // ������ ����� � ��������
	float   cwy;                                        // ������ ����� � ��������

	// ��������� ������ � ������
	// �� ��������� �������: 16�16
	cwx         = (1.0f / 256.0f) * 16;
	cwy         = (1.0f / 256.0f) * 16;

	Font.Base=glGenLists(256);								// ������ 256 ������� ������
	for (int loop=0; loop<256; loop++)						// �������� �� ���� ������
	{
		// X � Y �������
		cx=float(loop%16)/16.0f;
		cy=float(loop/16)/16.0f;
		glNewList(Font.Base+loop,GL_COMPILE);				// �������� ��������� ���� 
			glBegin(GL_QUADS);							// ������ ����� �����������������
				glTexCoord2f(cx+(Font.FTrim[loop][0])/256.0f,cy+cwy);			
				glVertex2i(0,16);
				glTexCoord2f(cx+cwx-(16-Font.FTrim[loop][0]-Font.FTrim[loop][1])/256.0f,cy+cwy);
				glVertex2i(Font.FTrim[loop][1],16);
				glTexCoord2f(cx+cwx-(16-Font.FTrim[loop][0]-Font.FTrim[loop][1])/256.0f,cy);
				glVertex2i(Font.FTrim[loop][1],0);
				glTexCoord2f(cx+(Font.FTrim[loop][0])/256.0f,cy);
				glVertex2i(0,0);
			glEnd();									// ��������� ��������
			// ������������ �� ������ �������
			glTranslated(Font.FTrim[loop][0]+Font.FTrim[loop][1]+Font.Spacing,0,0);
			
		glEndList();									// ��������� ����
	}													// ���� �� ������
	
	return TRUE;
}

// ������� �����
void gunRender::KillFont(gunFont &Font)
{
	// ������� ��� ����� (�����)
	glDeleteLists(Font.Base, 256);
}

//������� ������ ������ � ���������� ������
// x,y - �������
// Text - �� �������� � printf
void gunPrint2D(float x,float y,const char *Text,...)
{
	va_list		va;									// ������ �� ������ ����������
	if(Text=="")									// ���� ���� ������
		return;										// ������ �� ������

	// �������� ����� � buf, ������� ���������� �� ����������
	va_start(va, Text);
	vsprintf(buf, Text, va);
	va_end(va);

	// ������������� ����
	glColor4ubv(&Font[MainRender.CurFont].Color.Color[0]);
	// �������� ��������
	MainRender.TextureEnable(Font[MainRender.CurFont].TextureID);
	// ��������� ������� 2D ������
	MainRender.RenderMode(GUN_RENDERMODE_2D);
	// ��������� ������� �������
	glPushMatrix();
	// ������ � ��������
	glLoadIdentity();
	// �����������, � ����������� �� ������ � ������ ����
	glScalef(Font[MainRender.CurFont].Width,Font[MainRender.CurFont].Height, 1);
	// ���������� �� x � y
	glTranslated(x/Font[MainRender.CurFont].Width,y/Font[MainRender.CurFont].Height,0);
	// ������� ���� ������ �� �������
	glListBase(Font[MainRender.CurFont].Base);
	// ������ ���� ����� ������� ��������������� ������
	glCallLists((GLsizei)strlen(buf), GL_UNSIGNED_BYTE, buf);
	// �������������� ������� �������
	glPopMatrix();


}

// ��������� ������
// 1. GUN_RENDERMODE_3D - ��������� �����, ������� ���������
// 2. GUN_RENDERMODE_2D - ���������� �����, �����, ������� � �.�.
// 3. GUN_RENDERMODE_RESTORE_LAST - �������������� ���������� �����
void gunRender::RenderMode(gunEnum mode)
{
	switch (mode)
	{
	case GUN_RENDERMODE_3D:
		// ��������� ������� �����
		if (CurrentRenderMode!=GUN_RENDERMODE_3D)
		{
			// ����������� ����������
			RenderMode(GUN_RENDERMODE_RESTORE_LAST);
			// ���� ��� ������, �� �������
			CurrentRenderMode=GUN_RENDERMODE_3D;
		}
		break;
	case GUN_RENDERMODE_2D:
		// ��������� ������� �����
		if (CurrentRenderMode!=GUN_RENDERMODE_2D)
		{
			// ����������� ����������
			RenderMode(GUN_RENDERMODE_RESTORE_LAST);
			// ��������� ��������
			glPushAttrib(GL_BLEND | GL_DRAW_BUFFER | GL_COLOR_BUFFER_BIT |
				GL_LOGIC_OP | GL_DEPTH_BUFFER_BIT |
				GL_DEPTH_TEST | GL_ENABLE_BIT | GL_LIGHTING_BIT | 
				GL_STENCIL_BUFFER_BIT );
			// ��������� ������� ��������
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			// ������ � ���������
			glLoadIdentity();
			// ������������ 2D �����, � ����������� �� �������� ��������
			glOrtho(0,FontDisplayWidth,FontDisplayWidth,0,-100,100);
			// ��������� ������� �������
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			// ������ � ���������
			glLoadIdentity();
			// ��������� ���� �������
			glDisable(GL_DEPTH_TEST);
			// �������� ������������
			glBlendFunc(GL_ONE,GL_ONE);
			// ��������� ������ �  ������ �������
			glDepthMask(FALSE);
			// ������������� ������� �����
			CurrentRenderMode=GUN_RENDERMODE_2D;
		}
		break;
	case GUN_RENDERMODE_RESTORE_LAST:
		// ���� ����� 2D
		if (CurrentRenderMode==GUN_RENDERMODE_2D)
		{
			// �������������� ������� ��������
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			// �������������� ������� �������
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			// �������������� ��������
			glPopAttrib();
		}
		// ������� �����: �����
		CurrentRenderMode=GUN_NONE;
		break;
	}
}

// ������������� ���������� fps (������ � �������)
// Time - ����� � ��������, �������� ����� ����������� fps
void gunRender::StartCalculateFPS(float Time)
{
	// fps �����������
	ISfps=TRUE;
	fps=0;
	// �������� ����� �����
	LastFPSFrame=Frame_Num;
	// �������� �������
	FPSTimer.gunStartTiming();
	// �������� � ����� ����� ������������
	FPSTime=(UINT64)Time*10000;
}

//���������� fps
void gunRender::UpdateFPS(UINT64 &DeltaTime)
{
	// ���� fps �� �����������, �� �������
	if (!ISfps) return;
	// ������� ��������� �����
	FPSTimer.gunTimeElapsed();
	// ���� ��������� ����� ������ ���������
	if (FPSTimer.ElapsedTime>FPSTime)
	{
		// ��������� fps
		// �����������_������*10000/���������_�����
		fps=(UINT)((Frame_Num-LastFPSFrame)*10000.f/FPSTimer.ElapsedTime);
		// �������� �������
		FPSTimer.gunStartTiming();
		// ���������� ����
		LastFPSFrame=Frame_Num;
	}
}