#pragma once
#include "D:\\Движки\freeglut-2.8.1\include\GL\freeglut.h"
#include <iostream>
#include <time.h>
#include "Quad.h"
using namespace std;

namespace Fruct
{

	class Fructs_ 
{

public:

	Fructs_(int& N, int& M, int& Scale_, int& startX, int& startY);
	Fructs_();
	Fructs_(const Fructs_& copyConstructor);
	~Fructs_();

	typedef struct 
	{
       int x,y;
	} vec;
	 
	       void DrawApple();
           
	 const bool GetActive();
	       void StartDraw();
 
	Fructs_& GetXY();
	void SetLive(const bool& active);
    void New();
	vec m_vec[10];
	static bool visibleFructs[10];
	void SetVisible(const bool& set, const int& indx);
	bool GetVisible(const int& indx) const;
		inline void box_(float x1, float y1, float x2, float y2);
     static int  countRun;
	  void SetDepthFruct(const float depth);
	 float GetDepthFruct() const;


 

private:


     float frame;
	float depthFruct;

	bool isActive;
    int sizeWall;
	int m_scale;
	int m_N,m_M;

 
} ;
}

