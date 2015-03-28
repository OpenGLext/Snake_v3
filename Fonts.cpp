#include "Fonts.h"
unsigned int FontBase;              // База отображаемого списка для набора символов 
GLYPHMETRICSFLOAT gmf[256];  // Массив с информацией о нашем шрифте

//А для того чтобы текст был на экране а не в 3d:
//        CRect rect; 
//GetClientRect(&rect);
//
//glMatrixMode(GL_PROJECTION); 
//glLoadIdentity(); 
//gluOrtho2D(0,rect.left,0,rect.top); 
//glMatrixMode(GL_MODELVIEW); 
//glLoadIdentity();
//
//        рисуем текст...

void InitFonts() 
{ 
HFONT  font;                                  // Идентификатор шрифта Windows 
FontBase = glGenLists(256);        // массив для 256 букв
font = CreateFont(-12,          // высота шрифта

              0,            // ширина знакоместа 
              0,            // Угол перехода 
              0,            // Угол направления 
  FW_SEMIBOLD,  // Ширина шрифта 
              FALSE,        // Курсив 
              FALSE,        // Подчеркивание 
              FALSE,        // Перечеркивание 
              ANSI_CHARSET,	// Идентификатор кодировки 
              OUT_TT_PRECIS,	// Точность вывода 
              CLIP_DEFAULT_PRECIS,	// Точность отсечения 
              ANTIALIASED_QUALITY,	
              FF_DONTCARE|DEFAULT_PITCH,L"times");

SelectObject(wglGetCurrentDC(), font);

wglUseFontOutlines( wglGetCurrentDC(),    // Выбрать текущий контекст устройства (DC) 
              0,              // Стартовый символ 
              255,          // Количество создаваемых списков отображения 
              FontBase,    // Стартовое значение списка отображения 
  0.0f,      //Отклонение от настоящего контура 
              0.04f,      //Толщина шрифта по оси Z 
              WGL_FONT_POLYGONS,      //Использовать полигоны, а не линии 
  gmf);      //буфер адреса для данных списка отображения 
}



void glPrint_(const char *fmt, ... ) 
{ 
if (fmt == NULL)            // Если нет текста, 
return;              // ничего не делаем 
glPushMatrix();
float length=0; // Переменная для нахождения физической длины текста 
        char text[256]; // Здесь наша строка 
va_list ap;	// Указатель на переменный список аргументов

    va_start(ap, fmt);        // Анализ строки на переменные 
    vsprintf(text, fmt, ap);  // И конвертация символов в реальные коды 
    va_end(ap);                // Результат сохраняется в text

for (unsigned int loop=0;loop<strlen(text);loop++) //Цикл поиска размера строки 
    { 
length+=gmf[text[loop]].gmfCellIncX; // Увеличение размера на ширину символа 
    }

glTranslatef(-length/2,0.0f,0.0f);      //Центровка на экране нашей строки 
glPushAttrib(GL_LIST_BIT); // Сохраняет в стеке значения битов списка отображения 
glListBase(FontBase);          // Устанавливает базовый символ в 0 
glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); 
glPopAttrib(); // Восстанавливает значение Display List Bits 
glPopMatrix(); 
}



void CleanupFonts() 
{ 
if(FontBase) 
glDeleteLists(FontBase, 256); 
}