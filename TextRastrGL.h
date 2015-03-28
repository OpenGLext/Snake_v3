#pragma once
#include "D:\\Движки\freeglut-2.8.1\include\GL\freeglut.h"
#include <memory>

namespace TextRastr{

class TextRastrGL
{
public:
	TextRastrGL(void);
	~TextRastrGL(void);

	void PrintText(const char *fmt, ...);
	void SetPosition(const GLfloat& posX, const GLfloat& posY);
	void Text();

private:

	void BildFont();
	void KillFont();
	GLuint	base;
	GLfloat m_posx,m_posy;
	HDC	hDC;		
};

}
