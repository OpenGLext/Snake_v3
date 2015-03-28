#pragma once
#include "quad.h"
class Background 
{
public:
	Background(void);
	~Background(void);

	
   void DrawHorizontLine(const int n, const float pos_x, const float pos_y, const float scale_);
   void DrawLife();
   void DrawHigh();

	inline void box_(float x1, float y1, float x2, float y2);

	void SetTexture(Texture* texture);

	Quad *pQuad;

private:
	Texture * txt;

	
};

