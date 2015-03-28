#pragma once
#include "D:\\������\freeglut-2.8.1\include\GL\freeglut.h"
#include <stdio.h>											// Header File For Standard Input / Output
#include <stdarg.h>
#include "Quad.h"




	

GLuint	base;	

int LoadGLTextures()										
{
        int Status=FALSE;		
		
       Quad ren;

		char *fon_ = "Font.tga";
		

		
		ren.LoadTexture(&texture[17], fon_);
		

		if ( tf->imageData != NULL  )
        {
			Status=TRUE;									// Set The Status To TRUE

			glGenTextures(1, &tf->texID);
		

			
				glGenTextures(1, &tf->texID);
                glBindTexture(GL_TEXTURE_2D, tf->texID);

                gluBuild2DMipmaps(GL_TEXTURE_2D, tf->bpp / 8,  tf->width,  tf->height,
				                                 tf->type, GL_UNSIGNED_BYTE,  tf->imageData);		

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
                glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
			

		
		}
return Status;	
		}


GLvoid BuildFont(GLvoid)									// Build Our Font Display List
{
	base=glGenLists(256);									// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, tf->texID);				// Select Our Font Texture
	for (int loop1=0; loop1<256; loop1++)						// Loop Through All 256 Lists
	{
		float cx=float(loop1%16)/16.0f;						// X Position Of Current Character
		float cy=float(loop1/16)/16.0f;						// Y Position Of Current Character

		glNewList(base+loop1,GL_COMPILE);					// Start Building A List
			glBegin(GL_QUADS);								// Use A Quad For Each Character
				glTexCoord2f(cx,1.0f-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2d(0,16);							// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,16);							// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1.0f-cy);			// Texture Coord (Top Right)
				glVertex2i(16,0);							// Vertex Coord (Top Right)
				glTexCoord2f(cx,1.0f-cy);					// Texture Coord (Top Left)
				glVertex2i(0,0);							// Vertex Coord (Top Left)
			glEnd();										// Done Building Our Quad (Character)
			glTranslated(15,0,0);							// Move To The Right Of The Character
		glEndList();										// Done Building The Display List
	}														// Loop Until All 256 Are Built
}

GLvoid KillFont(GLvoid)										// Delete The Font From Memory
{
	glDeleteLists(base,256);								// Delete All 256 Display Lists
}

GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)	// Where The Printing Happens
{
	char		text[256];									// Holds Our String
	va_list		ap;											// Pointer To List Of Arguments

	if (fmt == NULL)										// If There's No Text
		return;												// Do Nothing

	va_start(ap, fmt);										// Parses The String For Variables
	    vsprintf(text, fmt, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);												// Results Are Stored In Text

	if (set>1)												// Did User Choose An Invalid Character Set?
	{
		set=1;												// If So, Select Set 1 (Italic)
	}
	glEnable(GL_TEXTURE_2D);								// Enable Texture Mapping
	glLoadIdentity();										// Reset The Modelview Matrix
	glTranslated(x,y,0);									// Position The Text (0,0 - Bottom Left)
	glListBase(base-32+(128*set));							// Choose The Font Set (0 or 1)

	if (set==0)												// If Set 0 Is Being Used Enlarge Font
	{
		glScalef(1.5f,2.0f,1.0f);							// Enlarge Font Width And Height
	}

	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);		// Write The Text To The Screen
	glDisable(GL_TEXTURE_2D);								// Disable Texture Mapping
}
