#pragma once
// Font.h
#include "D:\\Движки\freeglut-2.8.1\include\GL\freeglut.h"

class Font
{
protected:
  unsigned fontOffset;

public:
  Font(HDC hDC, char* fontFace, int width, int height, int weight);
  Font();
  ~Font();

  void put(float x, float y, float z, const char* str);
};

Font::Font()
{

}

// Font.cpp
Font::Font(HDC hDC, char* fontFace, int width, int height, int weight)
{
  HFONT hFont = CreateFontA(height, width, 0/*aspect angle*/,
    0/*angle*/,weight, FALSE/*italic*/, FALSE/*underline*/, FALSE/*strike-out*/, 
    RUSSIAN_CHARSET, OUT_TT_PRECIS/*render precision*/, CLIP_DEFAULT_PRECIS/*clip precision*/,
    ANTIALIASED_QUALITY/*quality*/, FF_DONTCARE|DEFAULT_PITCH, fontFace);
  if( hFont == NULL )
    return;

  fontOffset = glGenLists( 256 );
  HGDIOBJ old = SelectObject( hDC, hFont );
    wglUseFontBitmaps(hDC, 0, 256, fontOffset);
  SelectObject( hDC, old );
  DeleteObject( hFont );

  return;
}

Font::~Font()
{
  glDeleteLists(fontOffset, 256);
}

void Font::put(float x, float y, float z, const char *str)
{
  glRasterPos3f(x, y, z);

  glPushAttrib(GL_LIST_BIT);
  glListBase(fontOffset);
  glCallLists((GLsizei)strlen(str), GL_UNSIGNED_BYTE, str);
  glPopAttrib();
}

