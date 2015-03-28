#pragma once
#include "D:\\Движки\freeglut-2.8.1\include\GL\freeglut.h"
#include <stdio.h>
#include "DefineTexture.h"


using namespace std;


typedef struct
{
	GLubyte Header[12];									// TGA File Header
}   TGAHeader;

typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;

class Quad
{
public:

	Quad( );
	Quad(const Quad& q1);
	~Quad();

	
		float startx,starty,scale,scaleY;

	    
		void SetTexture(char* pathTexture="head.tga");

		
		void DrawVerticalLine(const int n=2, const float pos_x=100, const float pos_y=100, const float scale_=50.0f);
		bool Compare(const Quad& q1);
        void DrawHorizontLine(const int n=2, const float pos_x=100, const float pos_y=100, const float scale_=50.0f);
        void SetIdTexture(const int idTexture);
	    int  LoadTexture(Texture *TEXTURE, char *filename);
	    bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA);
		bool  Quad::LoadCompressedTGA(Texture * texture, char * filename, FILE * fTGA);
int id,n_;

   
inline void FiltringTexture(Texture *txt);

  inline void box(float x1, float y1, float x2, float y2);  

private:
		
		bool Destroy;
	    int maxElement; // кол-во разнотиповых фигур - разная текстура,форма и тп
		int countTexture;
		int otstup;
        bool LoadTGA(Texture * texture, char * filename);
        void RandTextureGen();
		
	
        //bool LoadCompressedTGA(Texture * texture[1], char * filename, FILE * fTGA);

	    TGAHeader *tgaheader;									
        TGA *tga;		
		GLubyte *uTGA;
		GLubyte *cTGA;
	    
		char * randTexture;

    	
};

