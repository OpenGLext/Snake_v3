#pragma once
#include <sstream>
#include "Fructs_.h"
#include "StructData.h"

#include "Quad.h"
#include "Background.h"
#include "LogSystem.h"
#include "Snake_Obj.h"
#include "Font.h"
#include "DefineTexture.h"
#include "Texturing.h"
#include "Fonts.h"
#include <windef.h>
#include <atltypes.h>
#include "Fonts_blockDestroyer.h"
 
using namespace LogSpace;
using namespace Data;

inline void Times()
{
	//__int64 time;											// time Will Hold A 64 Bit Integer
	//QueryPerformanceCounter((LARGE_INTEGER *) &time);	// Grab The Current Performance Time
	int time;
	time = GetTickCount();
	CurFPS++;

	if( time - oldtime > 1000 )
	{
		oldtime = time;
		FPS = CurFPS;
		CurFPS = 0;
	}
	spd = (float)100 / (float)FPS;
	if ( spd < 2.0f) spd = 1.0f;
	//Pauseval -= 1.0f * spd ;            // 20.0 - 2..4..6
}
std::string GetStr(const char* mess,const float value,const int valueInt=0)
{
	std::string ScoreFloat="";
	std::stringstream StrStream;

	StrStream<<value;
	StrStream>>ScoreFloat;
	ScoreFloat = mess + ScoreFloat ;
	 
	return ScoreFloat;
}
 void ResetGame()
 {

	 glClearColor(0,0,0,0);
	 glClear(GL_COLOR_BUFFER_BIT);
 }
 // ≈щЄ одна нова€ функци€: создаЄт пустую текстуру дл€ рендера в неЄ
 void CreateRenderTexture(Texture textureArray, int size, int channels, int type, int textureID)
{
    // указатель дл€ сохранени€ данных текстуры
    unsigned int *pTexture = NULL;

    // Ќужно создать пустую текстуру дл€ рендера динамической текстуры.
    // „тобы сделать это, просто создаем массив дл€ хранине€ данных и
    // передаем его OpenGL. “екстура хранитс€ в видеокарте, так что мы можем
    // уничтожить массив в любой момент.
    // Ёта функци€ принимает текстурный массив дл€ сохранени€ текстуры,
    // размер текстуры, высоту и ширину, каналы (1,3,4), тип (RGB, RGBA, etc)
    // и текстурный ID.

    // »нициализируем пам€ть под массив текстуры и прив€жем к pTexture
    pTexture = new unsigned int [size * size * channels];
    memset(pTexture, 0, size * size * channels * sizeof(unsigned int));

    // «арегистрируем текстуру в OpenGL и прив€жем к ID
    glGenTextures(1, &textureArray.texID);
    glBindTexture(GL_TEXTURE_2D, textureArray.texID);

    // —оздаем текстуру и сохран€ем в пам€ти
    glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, pTexture);

    // ”станавливаем качество
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // —охранив текстуру с пом. OpenGL, мы можем удалить временные данные
    delete [] pTexture;
}
 void OutLogotype()
 {
	 Font *ourtext = new Font(hDC,"times",15,25,30);
	 if (ourtext == NULL ) { MessageBox(0,L"ourtext is null",L"info",MB_OK);}


  glColor3f(0.0f,0.0f,0.3f);
  ourtext->put(250, 460, 1.0f, "Snake v3.5");
 
  delete ourtext;
 }
 void GraphicLog(const char *mess)
 { 
  Font *ourtext = new Font(hDC,"times",15,25,30);
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.0f,0.0f,0.9f);
  ourtext->put(20, 430, depthGraphLog, mess);
  delete ourtext;
  glEnable(GL_TEXTURE_2D);
 }
 void DrawBackground()
 {
	
  fon.DrawHorizontLine(0,0,0,0);

		
 }
 void OutHigh()
 {
	 
	
	// if (ourtext == NULL ) { MessageBox(0,L"ourtext is null",L"info",MB_OK);}

    std::string ScoreStr="Score: ";
	std::string ScoreInt="";
	std::string levelStr="Level: ";
	std::string levelInt="";

	std::stringstream StrStream, levelStream;
	StrStream<<Data::high;
	StrStream>>ScoreInt;
	ScoreStr += ScoreInt;
	
	levelStream<<Data::Level;
	levelStream>>levelInt;
	levelStr += levelInt;
	InitFonts();

	//ј дл€ того чтобы текст был на экране а не в 3d:
//RECT rect;
//rect.bottom = 300;
//rect.left = 0;
//rect.right = 400;
//rect.top = 0;
//GetClientRect(0,&rect);


//glMatrixMode(GL_PROJECTION); 
//glLoadIdentity(); 
//gluOrtho2D(10,10,0,0); 
//glMatrixMode(GL_MODELVIEW); 
//glLoadIdentity();

//        рисуем текст...

	glRasterPos2f(10.0f,10.0f);
	glPrint_(levelStr.c_str());
	
          //glColor3f(0.0f,0.7f,0.5f);
	    //  ourtext->put(490, 390, 0.7f, ScoreStr.c_str());
		  //ourtext->put(490, 340, 0.8f, levelStr.c_str());
      
 }
 void DrawSnake()
 {
 
	//snake.SetTimeDraw(spd);
	/*std::string str = GetStr("FPS",0,FPS);
	glColor3f(0.0f,1.0f,0.0f);
	glRasterPos2d(100,100);
	glPrint(100,100,1,str.c_str());*/
    BindingTexture(head);
    snake.Draw();
 
 }
 void DrawFruct()
 {
	/* retFructDepth=fructs[0].GetDepthFruct();
	 std::string str = GetStr("keys depth fruct o[-],p[+] : ",retFructDepth);
	 GraphicLog(str.c_str());*/

    BindingTexture(fruct);
       fructs[0].DrawApple(); 
	   
 }
 void LoadGameOver()
{
	      //glEnable(GL_BLEND);
		  //glBlendFunc();

         // glDisable(GL_TEXTURE_2D);
		
          glColor3f(1.0f,0.0f,0.0f);
	     // ourtext.put(250, 250, 0.0f, "Game Over");

		  Data::GameOver = false;
          snake.SetLenghtBody(4); 

		  //glColor3f(0.0f,0.0f,1.0f);
		  //ourtext_.put(150,15,0.0f,"Press space to Start game or ESC to Exit");
		 // glEnable(GL_TEXTURE_2D);

		
}
 void LevelComplete()
 {
	   
    ResetGame(); 
		 // glClearColor(0.0f,0.5f,0.5f,0.0f);
	
	     // ourtext.put(150, 250, 0.0f, "Level is complete...");
		
 }
 void ResetLives()
  {
	  if ( Data::lives == 0) { LoadGameOver(); }
	  else Data::lives -= 1;
  }
 void OutLives()
  {

  }
 inline GLvoid box(float x1, float y1, float x2, float y2)
{
	glBegin(GL_QUADS);	
        	glTexCoord2f(0.0f, 0.0f); glVertex2f(x1,y1);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(x2,y1);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(x2,y2);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(x1,y2);
			
	glEnd();
}
 void Draw()
{   
	glEnable(GL_TEXTURE_2D); 
	ResetGame(); 

	
	/* for(int i=5; i<1; i--)
	 {*/

        Sleep(30);
        Times();
		 Sleep(30);

	 if (!Data::GameOver) 
		 { 
	
			//OutLives(); 
	        DrawBackground();
            DrawSnake();
            DrawFruct(); 
			 //OutLogotype();
			// OutHigh();
			
	//if ( snake.IsCollision()) { Data::high += 100; snake.SetCollision(false); }
	if ( snake.IsGameOver()) 
	     { 
			ResetLives();   
	     }
	if ( snake.GetLenBody() == Data::countFructs) { if (Data::Level == 3) { Data::Level = 1; } else  Data::Level += 1;
	     snake.SetLenghtBody(4); fructs[0].New(); OutHigh(); LevelComplete(); }
	//добавить остановку игры как в гейм овер
	 
	    // }
	 }

	/* if (( !Data::Starting) && (Data::GameOver))
	 {
		 LoadGameOver();
	 }*/

}
 int DrawLogo(float n)
{
	glEnable(GL_TEXTURE_2D);
	BindingTexture(logo);
	//glBindTexture(GL_TEXTURE_2D, logo->texID);			// logo.tga
	box(0.0f,0.0f,Data::wf,Data::hf);	
	

	//if (n>=1){ // если конец игры вывести рекорд таблицу
		//glColor3f(1.0f,1.0f,1.0f);								// Set Color To Purple
		//if ((GameTime/100) % 2==0) 
			//glPrint(300,600,0,"press button to menu");							// Write GRID CRAZY On The Screen
	//}
	return TRUE;											
}
 int DrawMenu(float n,int ID)
{
	// BACKGROUND
	BindingTexture(fonMenu);		// backmenu.tga
	//box(20.0f-400*(1-n),120.0f,330.0f-400*(1-n),360.0f);	
	box(300,150,450,300);

	// MENU ITEMS
	BindingTexture(buttonMenu);		// button.tga
	for (int i=0;i<4;i++)
	{
	if (i!=ID) if (menu_items_fade[i]>0.5f){menu_items_fade[i]-=0.002f; }
	if (ID==i) if (menu_items_fade[i]<1.0f){menu_items_fade[i]+=0.008f; } 
	glColor3f(menu_items_fade[i],menu_items_fade[i],menu_items_fade[i]);
	
	//box(40.0f-400*(1-n),150.0f+i*50,300.0f-400*(1-n),195.0f+i*50);	
	glColor4f(0.0f,0.0f,0.0f,0.5f);
	box(320,280,430,320);
	}

    BindingTexture(fonMenu);		// backmenu.tga
	for (int i=0;i<4;i++)
	{
		glColor3f(menu_items_fade[i],menu_items_fade[i],menu_items_fade[i]);
		//glPrint(50.0f-400.0f*(1.0f-n),155.0f+i*50.0f,0,menu_items[i]);							// Write GRID CRAZY On The Screen
	}
	
	return TRUE;											
}
 void ProcessMenu()
 {

	


	
	 if ( CurMenu == LOGO) { DrawLogo(menufade[LOGO]); }
	 if ( CurMenu == MAIN_MENU)  {  DrawMenu(menufade[MAIN_MENU],0); }
	 if ( CurMenu == GAME)  {  Draw(); }
	 

	glutSwapBuffers(); 
	glDisable(GL_TEXTURE_2D);
 }

