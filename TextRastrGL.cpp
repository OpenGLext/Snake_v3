#include "TextRastrGL.h"
using namespace TextRastr;

TextRastrGL::TextRastrGL(void)
{
	this->BildFont();
	m_posx = 100; m_posy = 100;
	hDC=NULL;
}


TextRastrGL::~TextRastrGL(void)
{
	this->KillFont();
}

void TextRastrGL::BildFont()
{
	HFONT  m_hFont;
 
    base = glGenLists(256);          // массив для 256 букв
 
    m_hFont = CreateFont( -24,              // Высота фонта
                            0,              // Ширина фонта
                            0,              // Угол отношения
                            0,              // Угол наклона
                            FW_BOLD,        // Ширина шрифта
                            FALSE,          // Курсив
                            FALSE,          // Подчеркивание
                            FALSE,          // Перечеркивание
                            ANSI_CHARSET,   // Идентификатор набора символов
                            OUT_TT_PRECIS,  // Точность вывода
                            CLIP_DEFAULT_PRECIS,        // Точность отсечения
                            ANTIALIASED_QUALITY,        // Качество вывода
                            FF_DONTCARE|DEFAULT_PITCH,  // Семейство и шаг
                            L"Arial");                   // Имя шрифта
 
    SelectObject(hDC, m_hFont);
    wglUseFontBitmaps(hDC, 32, 96, base);// Построить 96 символов, начиная с пробела	

}

void TextRastrGL::KillFont()
{
	glDeleteLists(base, 256);
}

void TextRastrGL::SetPosition(const GLfloat& posX, const GLfloat& posY)
{
	m_posx = posX; m_posy = posY;

}

void TextRastrGL::Text()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
 
    glRasterPos3d(10, 10, 0);
    PrintText("Game Time: %6.1f", 500);
    glRasterPos3d(-3.65, 1.4, -5);
    PrintText("Speed: %14.1f",900);
}


void TextRastrGL::PrintText(const char *fmt, ...)
{
	//if (!fmt) return false;
        
    char text[256];
    va_list    ap;
 
    va_start(ap, fmt);
	vsprintf(text,fmt,ap);
    va_end(ap);
 
    glPushAttrib(GL_LIST_BIT);
    glListBase(base - 32);
 
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();


}