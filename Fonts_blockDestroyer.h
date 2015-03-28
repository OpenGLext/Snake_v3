#pragma once


GLvoid BuildFont(GLvoid)									// Build Our Font Display List
{
	fontbase=glGenLists(256);									// Creating 256 Display Lists
	//glBindTexture(GL_TEXTURE_2D, fontBlockdestroyer->texID);				// Select Our Font Texture
	BindingText(fontBlockdestroyer);

	for (int i=0; i<256; i++)						// Loop Through All 256 Lists
	{
		float cx=float(i%16)/16.0f;						// X Position Of Current Character
		float cy=float(i/16)/16.0f;						// Y Position Of Current Character

		glNewList(fontbase+i,GL_COMPILE);					// Start Building A List
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
	glDeleteLists(fontbase,256);								// Delete All 256 Display Lists
}

GLvoid glPrint(GLfloat x, GLfloat y, int set, const char *fmt, ...)	// Where The Printing Happens
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
    //glBindTexture(GL_TEXTURE_2D, fontBlockdestroyer->texID);			// ( CHANGE )
	BindingText(fontBlockdestroyer);
	glPushMatrix();
	glLoadIdentity();										// Reset The Modelview Matrix
	glTranslated(x,y,0);									// Position The Text (0,0 - Bottom Left)
	glListBase(fontbase-32+(128*set));							// Choose The Font Set (0 or 1)

	if (set==0)												// If Set 0 Is Being Used Enlarge Font
	{
		glScalef(1.5f,2.0f,1.0f);							// Enlarge Font Width And Height
	}

	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);		// Write The Text To The Screen
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);								// Disable Texture Mapping
}