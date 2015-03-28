#pragma once
#include "D:\\Движки\freeglut-2.8.1\include\GL\freeglut.h"

#include "Quad.h"
#include "Background.h"
#include "LogSystem.h"
#include "Snake_Obj.h"



 int Fruct::Fructs_::countRun = 0;
 bool Fruct::Fructs_::visibleFructs[10] = { true };
 bool SNAKE::Snake_Obj::GameOver = false;
  
  Texture *texture = new Texture;
  Texture *fon_ = new Texture;
  Texture *fruct = new Texture;
  Texture *head = new Texture;
  Texture *body = new Texture;
  Texture *tail = new Texture;
  Texture *fonts = new Texture;
  Texture *life = new Texture;

  Texture *logo = new Texture;
  Texture *buttonMenu = new Texture;
  Texture *fonMenu = new Texture;

  Texture *RenderTextureFon = new Texture;
  Texture *RenderTextureSnake = new Texture;
  Texture *RenderTextureFruct = new Texture;
  Texture *fontBlockdestroyer = new Texture;
  
  GLuint	    fontbase;

namespace Data
{
	
const int MAIN_MENU=1;
const int OPTIONS=2;
const int HI_SCORES=3;
const int EXIT_GAME=4;
const int EXIT_MENU=6;
const int GAME_OVER=7;
const int GAME=5;
const int LOGO=0;
int   CurMenu=0;
float menufade[5];
bool keys[256];
float   menu_items_fade[4]={0.5f,0.5f,0.5f,0.5f};
char    menu_items[4][15]={"New Game","Options","High scores","Exit"};

int	  FPS=100;
int   CurFPS=100;
__int64 oldtime=1;
float	spd=1.0f;

       int Scale = 25;
       int wf = Scale *30;
       int hf = Scale * 20;
       int IntervalTimer = 50;
       int IntervalBonus = 3000;
	   int countFructs = 6;
	   int LevelSpeed = 0;
	   int Level = 1;
	   HDC hDC;
	   int high = 0;
	   int lives = 3;

	   bool blend = false;
	   int id(0);
	   bool GameOver = false;
	   bool Winner = false;
	   bool Starting = false;
	   float depthScore = 0.2f;
	   float depthLevel = 0.3f;
	 

	    float depthFruct(0.7f),depthText(1.0f);
	   float stepdepth = 0.01f;
	   float retFructDepth;
	   float depthGraphLog = 0.9f;



  HFONT font;
 Fruct::Fructs_ fructs[10];
 Quad ldrTexture;
 Background fon;
 LogSpace::CLog logFile;
 SNAKE::Snake_Obj snake;



using namespace std;

}