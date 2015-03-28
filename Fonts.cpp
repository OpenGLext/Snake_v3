#include "Fonts.h"
unsigned int FontBase;              // ���� ������������� ������ ��� ������ �������� 
GLYPHMETRICSFLOAT gmf[256];  // ������ � ����������� � ����� ������

//� ��� ���� ����� ����� ��� �� ������ � �� � 3d:
//        CRect rect; 
//GetClientRect(&rect);
//
//glMatrixMode(GL_PROJECTION); 
//glLoadIdentity(); 
//gluOrtho2D(0,rect.left,0,rect.top); 
//glMatrixMode(GL_MODELVIEW); 
//glLoadIdentity();
//
//        ������ �����...

void InitFonts() 
{ 
HFONT  font;                                  // ������������� ������ Windows 
FontBase = glGenLists(256);        // ������ ��� 256 ����
font = CreateFont(-12,          // ������ ������

              0,            // ������ ���������� 
              0,            // ���� �������� 
              0,            // ���� ����������� 
  FW_SEMIBOLD,  // ������ ������ 
              FALSE,        // ������ 
              FALSE,        // ������������� 
              FALSE,        // �������������� 
              ANSI_CHARSET,	// ������������� ��������� 
              OUT_TT_PRECIS,	// �������� ������ 
              CLIP_DEFAULT_PRECIS,	// �������� ��������� 
              ANTIALIASED_QUALITY,	
              FF_DONTCARE|DEFAULT_PITCH,L"times");

SelectObject(wglGetCurrentDC(), font);

wglUseFontOutlines( wglGetCurrentDC(),    // ������� ������� �������� ���������� (DC) 
              0,              // ��������� ������ 
              255,          // ���������� ����������� ������� ����������� 
              FontBase,    // ��������� �������� ������ ����������� 
  0.0f,      //���������� �� ���������� ������� 
              0.04f,      //������� ������ �� ��� Z 
              WGL_FONT_POLYGONS,      //������������ ��������, � �� ����� 
  gmf);      //����� ������ ��� ������ ������ ����������� 
}



void glPrint_(const char *fmt, ... ) 
{ 
if (fmt == NULL)            // ���� ��� ������, 
return;              // ������ �� ������ 
glPushMatrix();
float length=0; // ���������� ��� ���������� ���������� ����� ������ 
        char text[256]; // ����� ���� ������ 
va_list ap;	// ��������� �� ���������� ������ ����������

    va_start(ap, fmt);        // ������ ������ �� ���������� 
    vsprintf(text, fmt, ap);  // � ����������� �������� � �������� ���� 
    va_end(ap);                // ��������� ����������� � text

for (unsigned int loop=0;loop<strlen(text);loop++) //���� ������ ������� ������ 
    { 
length+=gmf[text[loop]].gmfCellIncX; // ���������� ������� �� ������ ������� 
    }

glTranslatef(-length/2,0.0f,0.0f);      //��������� �� ������ ����� ������ 
glPushAttrib(GL_LIST_BIT); // ��������� � ����� �������� ����� ������ ����������� 
glListBase(FontBase);          // ������������� ������� ������ � 0 
glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); 
glPopAttrib(); // ��������������� �������� Display List Bits 
glPopMatrix(); 
}



void CleanupFonts() 
{ 
if(FontBase) 
glDeleteLists(FontBase, 256); 
}