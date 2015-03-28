#include "Quad.h"



Quad::Quad(void): maxElement(2),cTGA(NULL),uTGA(NULL),tgaheader(NULL),tga(NULL),otstup(2)
{
	
	   Texture *textures = new Texture;

	  // this->FiltringTexture();

	   

}
Quad::~Quad()
{
	//delete textures;
	//delete [] uTGA;
	//delete [] cTGA;
	//delete tga;
	//delete tgaheader;
}

Quad::Quad(const Quad& q1)
{

}

bool Quad::Compare(const Quad& q1)
{
	if ( q1.id == this->id ) return true;
	else return false;

}

inline void Quad::box(float x1, float y1, float x2, float y2)
{

	glBegin(GL_QUADS);	
			
			glTexCoord2f(0.0f, 0.0f); glVertex2f(x1,y1);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(x2,y1);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(x2,y2);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(x1,y2);


	glEnd();

}

void Quad::RandTextureGen()
{	

		maxElement =4; // для начала потренируемся на двух кубиках
	//this->id = rand()%maxElement; // исключать надо 0 начинаем считать с 1!!!!
		//id=10;
	countTexture = 2;

		switch (this->id)
		{
		
	    case 1 : randTexture = "box_0.tga"; break;
	    case 2 : randTexture = "box_1.tga"; break;
		case 3 : randTexture = "box_2.tga"; break;
		case 4 : randTexture = "box_3.tga"; break;
		case 5 : randTexture = "head.tga"; break;
		case 6 : randTexture = "body.tga"; break;
		case 7 : randTexture = "tail.tga"; break;
		case 0 : randTexture = "body.tga"; break;
		case 8 : randTexture = "stone.tga"; break;
		case 9:  randTexture= "apple.tga"; break;
		case 10: randTexture = "skin.tga"; break;
		case 11: randTexture = "background.tga"; break;
		case 12: randTexture = "newfon.tga"; break;
		//case 1 : randTexture = "tail.tga"; break;
		//case 2 : randTexture = "head.tga"; break;
		}

		this->LoadTexture(texture,randTexture);
}

void Quad::SetIdTexture(const int idTexture)
{
	id = idTexture;
}

void Quad::SetTexture(char* pathTexture)
{ 
	char *empty=" ";
	this->LoadTexture(texture,pathTexture); 
	if (pathTexture != empty) {randTexture = pathTexture;}
	else randTexture = "head.tga";
	
}
void Quad::DrawHorizontLine(const int n, const float pos_x, const float pos_y, const float scale_)
{

char*s="";

	if (texture->imageData == NULL) {this->RandTextureGen();}
	else if (randTexture[0] != *s ) { this->LoadTexture(texture,randTexture); }

	if (scale !=0) {scale = scale_;}
	else scale = 50.0f;

	if (pos_x !=0 ) { startx = pos_x; }
	else startx = 100.0f;
	if (pos_y !=0 ) { starty = pos_y;  }
	else starty = 100.0f;

	if (n!=0) {n_ = n;}
	else n_ = 2;

	

	for(int i=0; i<n_; i++)
	{
		/*if (nameTexture == "")
		{
		this->RandTextureGen();
		}
		else*/
		startx += scale;//+otstup;
		this->box(startx,starty, (startx+scale), (starty+scale) );
		this->LoadTexture(texture,randTexture);
		this->FiltringTexture(texture);
	}
}

void Quad::DrawVerticalLine(const int n, const float pos_x, const float pos_y, const float scale_)
{ 
	char*s="";

	if (texture == NULL) {this->RandTextureGen();}
	else if (randTexture[0] != *s ) { this->LoadTexture(texture,randTexture); }

	if (scale !=0) {scale = scale_;}
	else scale = 50.0f;

	if (pos_x !=0 ) { startx = pos_x; }
	else startx = 100.0f;
	if (pos_y !=0 ) { starty = pos_y;  }
	else starty = 100.0f;

	if (n!=0) {n_ = n;}
	else n_ = 2;

	glEnable(GL_TEXTURE_2D);

	for(int i=0; i<n_; i++)
	{
		/*if (nameTexture == "")
		{
		this->RandTextureGen();
		}
		else*/
		starty += scale+5;
		this->box(startx,starty, (startx+scale), (starty+scale) );
		this->LoadTexture(texture,randTexture);
		this->FiltringTexture(texture);
	}

}

bool  Quad::LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)
{
	TGA *tga = new TGA ;
    TGAHeader* tgaheader = new TGAHeader;

	if(fread(tga->header, sizeof(tga->header), 1, fTGA) == 0)					// Read TGA header
	{										
		MessageBox(NULL, L"Could not read info header", L"ERROR", MB_OK);		// Display error
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}	

	texture->width  = tga->header[1] * 256 + tga->header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga->header[3] * 256 + tga->header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga->header[4];										// Determine the bits per pixel
	tga->Width		= texture->width;										// Copy width into local structure						
	tga->Height		= texture->height;										// Copy height into local structure
	tga->Bpp			= texture->bpp;											// Copy BPP into local structure

	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	// Make sure all information is valid
	{
		MessageBox(NULL, L"Invalid texture information", L"ERROR", MB_OK);	// Display Error
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}

	if(texture->bpp == 24)													// If the BPP of the image is 24...
		texture->type	= GL_RGB;											// Set Image type to GL_RGB
	else																	// Else if its 32 BPP
		texture->type	= GL_RGBA;											// Set image type to GL_RGBA

	tga->bytesPerPixel	= (tga->Bpp / 8);									// Compute the number of BYTES per pixel
	tga->imageSize		= (tga->bytesPerPixel * tga->Width * tga->Height);		// Compute the total amout ofmemory needed to store data
	texture->imageData	= (GLubyte *)malloc(tga->imageSize);					// Allocate that much memory

	if(texture->imageData == NULL)											// If no space was allocated
	{
		MessageBox(NULL, L"Could not allocate memory for image", L"ERROR", MB_OK);	// Display Error
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if(fread(texture->imageData, 1, tga->imageSize, fTGA) != tga->imageSize)	// Attempt to read image data
	{
		MessageBox(NULL, L"Could not read image data", L"ERROR", MB_OK);		// Display Error
		if(texture->imageData != NULL)										// If imagedata has data in it
		{
			free(texture->imageData);										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	// Byte Swapping Optimized By Steve Thomas
	for(GLuint cswap = 0; cswap < (int)tga->imageSize; cswap += tga->bytesPerPixel)
	{
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		texture->imageData[cswap] ^= texture->imageData[cswap+2];
	}

	fclose(fTGA);
	delete tga; delete tgaheader;
	return true;								

}

bool  Quad::LoadCompressedTGA(Texture * texture, char * filename, FILE * fTGA)
{ 
	
	TGA *tga = new TGA ;
    TGAHeader* tgaheader = new TGAHeader;

	if(fread(tga->header, sizeof(tga->header), 1, fTGA) == 0)					// Attempt to read header
	{
		MessageBox(NULL, L"Could not read info header", L"ERROR", MB_OK);		// Display Error
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failed
	}

	texture->width  = tga->header[1] * 256 + tga->header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga->header[3] * 256 + tga->header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga->header[4];										// Determine Bits Per Pixel
	tga->Width		= texture->width;										// Copy width to local structure
	tga->Height		= texture->height;										// Copy width to local structure
	tga->Bpp			= texture->bpp;											// Copy width to local structure

	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	//Make sure all texture info is ok
	{
		MessageBox(NULL, L"Invalid texture information", L"ERROR", MB_OK);	// If it isnt...Display error
		if(fTGA != NULL)													// Check if file is open
		{
			fclose(fTGA);													// Ifit is, close it
		}
		return false;														// Return failed
	}

	if(texture->bpp == 24)													// If the BPP of the image is 24...
		texture->type	= GL_RGB;											// Set Image type to GL_RGB
	else																	// Else if its 32 BPP
		texture->type	= GL_RGBA;											// Set image type to GL_RGBA

	tga->bytesPerPixel	= (tga->Bpp / 8);									// Compute BYTES per pixel
	tga->imageSize		= (tga->bytesPerPixel * tga->Width * tga->Height);		// Compute amout of memory needed to store image
	texture->imageData	= (GLubyte *)malloc(tga->imageSize);					// Allocate that much memory

	if(texture->imageData == NULL)											// If it wasnt allocated correctly..
	{
		MessageBox(NULL, L"Could not allocate memory for image", L"ERROR", MB_OK);	// Display Error
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	GLuint pixelcount	= tga->Height * tga->Width;							// Nuber of pixels in the image
	GLuint currentpixel	= 0;												// Current pixel being read
	GLuint currentbyte	= 0;												// Current byte 
	GLubyte * colorbuffer = (GLubyte *)malloc(tga->bytesPerPixel);			// Storage for 1 pixel

	do
	{
		GLubyte chunkheader = 0;											// Storage for "chunk" header

		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)				// Read in the 1 byte header
		{
			MessageBox(NULL, L"Could not read RLE header", L"ERROR", MB_OK);	// Display Error
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(texture->imageData != NULL)									// If there is stored image data
			{
				free(texture->imageData);									// Delete image data
			}
			return false;													// Return failed
		}

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{
				if(fread(colorbuffer, 1, tga->bytesPerPixel, fTGA) != tga->bytesPerPixel) // Try to read 1 pixel
				{
					MessageBox(NULL, L"Could not read image data", L"ERROR", MB_OK);		// IF we cant, display an error

					if(fTGA != NULL)													// See if file is open
					{
						fclose(fTGA);													// If so, close file
					}

					if(colorbuffer != NULL)												// See if colorbuffer has data in it
					{
						free(colorbuffer);												// If so, delete it
					}

					if(texture->imageData != NULL)										// See if there is stored Image data
					{
						free(texture->imageData);										// If so, delete it too
					}

					return false;														// Return failed
				}
																						// write to memory
				texture->imageData[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga->bytesPerPixel == 4)												// if its a 32 bpp image
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
				}

				currentbyte += tga->bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
				{
					MessageBox(NULL, L"Too many pixels read", L"ERROR", NULL);			// if there is too many... Display an error!

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(texture->imageData != NULL)										// If there is Image data
					{
						free(texture->imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
			if(fread(colorbuffer, 1, tga->bytesPerPixel, fTGA) != tga->bytesPerPixel)		// Attempt to read following color values
			{	
				MessageBox(NULL, L"Could not read from file", L"ERROR", MB_OK);			// If attempt fails.. Display error (again)

				if(fTGA != NULL)														// If thereis a file open
				{
					fclose(fTGA);														// Close it
				}

				if(colorbuffer != NULL)													// If there is data in the colorbuffer
				{
					free(colorbuffer);													// delete it
				}

				if(texture->imageData != NULL)											// If thereis image data
				{
					free(texture->imageData);											// delete it
				}

				return false;															// return failed
			}

			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
			{																			// by the header
				texture->imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga->bytesPerPixel == 4)												// If TGA images is 32 bpp
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
				}

				currentbyte += tga->bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{
					MessageBox(NULL, L"Too many pixels read", L"ERROR", NULL);			// if there is too many... Display an error!

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(texture->imageData != NULL)										// If there is Image data
					{
						free(texture->imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
	}

	while(currentpixel < pixelcount);													// Loop while there are still pixels left
	fclose(fTGA);	
	delete tga; delete tgaheader;
	return true;						
}

bool  Quad::LoadTGA(Texture * texture, char * filename)
{
	GLubyte *cTGA = new GLubyte[12];
	GLubyte *uTGA = new GLubyte[12];

	uTGA[0] = 0; uTGA[1] = 0; uTGA[2] = 2; uTGA[3] = 0; uTGA[4] = 0;
	uTGA[5] = 0; uTGA[6] = 0; uTGA[7] = 0; uTGA[8] = 0; uTGA[9] = 0;
	uTGA[10] = 0; uTGA[11] = 0;

	cTGA[0] = 0; cTGA[1] = 0; cTGA[2] = 10; cTGA[3] = 0; cTGA[4] = 0;
	cTGA[5] = 0; cTGA[6] = 0; cTGA[7] = 0; cTGA[8] = 0; cTGA[9] = 0;
	cTGA[10] = 0; cTGA[11] = 0;

	FILE *fTGA;
		
	fopen_s(&fTGA,filename,"rb");

	//fTGA = fopen(filename, "rb");								// Open file for reading

	if(fTGA == NULL)											// If it didn't open....
	{
		MessageBox(NULL, L"Could not open texture file", L"ERROR", MB_OK);

		return false;														// Exit function
	}
	

	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)					// Attempt to read 12 byte header from file
	{
		MessageBox(NULL, L"Could not read file header", L"ERROR", MB_OK);		// If it fails, display an error message 
		if(fTGA != NULL)													// Check to seeiffile is still open
		{
			fclose(fTGA);	
	
		}
		return false;														// Exit function
	}

	if(memcmp(uTGA, &tgaheader, sizeof(tgaheader)) == 0)				// See if header matches the predefined header of 
	{	

	

		this->LoadUncompressedTGA(texture, filename, fTGA);						// If so, jump to Uncompressed TGA loading code
	}
	else if(memcmp(cTGA, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
	{	

	

		this->LoadCompressedTGA(&texture[1], filename, fTGA);							// If so, jump to Compressed TGA loading code
	}
	else																	// If header matches neither type
	{
		MessageBox(NULL, L"TGA file be type 2 or type 10 ", L"Invalid Image", MB_OK);	// Display an error
		fclose(fTGA);
	
		return false;																// Exit function
	}

	return true;

}

inline void Quad::FiltringTexture(Texture *txt)
{
	glGenTextures(1, &txt->texID);
glBindTexture(GL_TEXTURE_2D, txt->texID);

gluBuild2DMipmaps(GL_TEXTURE_2D, txt->bpp / 8, txt->width, txt->height, txt->type, 
                      GL_UNSIGNED_BYTE, txt->imageData);		
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//if (TEXTURE->imageData) free(TEXTURE->imageData);
}

int Quad::LoadTexture(Texture *TEXTURE, char *filename)
{

this->LoadTGA(TEXTURE, filename);

return true;

}

