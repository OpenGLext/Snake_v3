#pragma once
#include "Draws.h"
#include "StructData.h"
#include "LogSystem.h"

char *h    = "C:\\Data\\head.tga";
char *back = "C:\\Data\\backgroundTwo.tga";
char *fr   = "C:\\Data\\fruct.tga";
char *lf   = "C:\\Data\\life.tga";
char *logo_ = "C:\\Data\\logo.tga";
char *buttonMenu_ = "C:\\Data\\button.tga";
char *fonMenu_ = "C:\\Data\\backmenu-2.tga";
char *fontDestroyer_ = "C:\\Data\\font_blockdestroyer.tga";

void LoadTextures()
 {
   ldrTexture.LoadTexture(head, h); 
   //glGenTextures(1, &head->texID);

   ldrTexture.LoadTexture(fon_,back);
  // glGenTextures(1, &fon_->texID);

   ldrTexture.LoadTexture(fruct, fr);
   //glGenTextures(1, &fruct->texID);

  // BindingTexture(fruct);
   ldrTexture.LoadTexture(life, lf);

   ldrTexture.LoadTexture(logo,logo_);
   glGenTextures(1, &logo->texID);

   ldrTexture.LoadTexture(buttonMenu,buttonMenu_);

   ldrTexture.LoadTexture(fonMenu,fonMenu_);

   ldrTexture.LoadTexture(fontBlockdestroyer,fontDestroyer_);
   glGenTextures(1, &fontBlockdestroyer->texID);

 }
void InitGL()
{
	
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,Data::wf,0,Data::hf);
glEnable(GL_SMOOTH);
if (Data::blend )
{
glEnable(GL_BLEND); glBlendFunc(GL_ZERO,  GL_SRC_ALPHA_SATURATE);

}
else glDisable(GL_BLEND);

LoadTextures();
CurMenu = LOGO;
menufade[0] = 0.019f;
BuildFont();

 //glClearDepth(1.0f);									// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);			
 
}