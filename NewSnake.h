#pragma once
#include "GeometricRender.h"
#include "D:\\������\freeglut-2.8.1\include\GL\freeglut.h"

class NewSnake : public GeometricRender
{
public:
	NewSnake(void);
	~NewSnake(void);

	void Draw();
	void animDraw();
};

