#include "Background.h"

Background::Background(void)
{
	Quad *pQuad = new Quad;
}

Background::~Background(void)
{
	delete pQuad;
}

void Background::SetTexture(Texture *texture)
{
	txt = texture;
}

inline void  Background::box_(float x1, float y1, float x2, float y2)
{
	glBegin(GL_QUADS);	
			glTexCoord2f(0.0f, 0.0f); glVertex2f(x1,y1);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(x2,y1);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(x2,y2);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(x1,y2);
	glEnd();

}

void Background::DrawLife()
{
	this->box_(0,0,100, 100);
}

void Background::DrawHorizontLine(const int n, const float pos_x, const float pos_y, const float scale_)
{

    this->box_(0,0, 25*30, 20*30 ); 
	
	
	//this->FiltringTexture(texture);
	

//	glGenTextures(1, &texture[10].texID);
//    glBindTexture(GL_TEXTURE_2D, texture[10].texID);
//
//gluBuild2DMipmaps(GL_TEXTURE_2D, texture[10].bpp / 8,  texture[10].width,  texture[10].height,  texture[10].type, GL_UNSIGNED_BYTE,  texture[10].imageData);		
//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//		
//glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

}



