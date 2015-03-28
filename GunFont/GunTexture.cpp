/*********************************************************************************
*                                                                                *
*	GUNgine ������ 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#include "GunTexture.h"
#include "GunRender.h"

// ���������� Intel JPEG library, ��� ������ � jpg �������
#include "IJL\ijl.h"
#pragma comment( lib, "src\\IJL\\ijl15.lib" )

std::vector<gunTexture> Texture;

gunTexture::~gunTexture(void)
{
}

//������ �������� ���������� ������ � 3 ����������
void gunTexture::CreateEmpty(int width,int height, gunColor3ub &c)
{
	// ��������� ������ � ������
	Width=width;
	Height=height;
	// ��� �� �������
    Bpp=sizeof(gunColor3ub)*8;
	// �������� ����� ��� ��������
	Data.resize(width*height*sizeof(gunColor3ub));
	// ��������� �������� ������
	for (std::vector<BYTE>::iterator i=Data.begin();i<Data.end();i+=sizeof(gunColor3ub))
		memcpy(&*i,&c,sizeof(gunColor3ub));

}

void gunTexture::CreateEmpty(int width,int height, gunColor4ub &c)
{
	// ��������� ������ � ������
	Width=width;
	Height=height;
	// ��� �� �������
	Bpp=sizeof(gunColor3ub)*8;
	// �������� ����� ��� ��������
	Data.resize(width*height*sizeof(gunColor4ub));
	// ��������� �������� ������
	for (std::vector<BYTE>::iterator i=Data.begin();i<Data.end();i+=sizeof(gunColor4ub))
		memcpy(&*i,&c,sizeof(gunColor4ub));
}

//�������� �� �������� ����� ��� ����������
void ExtractFileExtension(const gunString &FileName,gunString &Ext)
{
	size_t i=FileName.rfind('.',FileName.size()-1);
	Ext=FileName.substr(i+1,FileName.size()-i-1);
	_strupr((char *)Ext.c_str());

}

// ��������� �������� �� �����
BOOL gunTexture::LoadFromFile(const gunString &FileName)
{
	gunString Ext;
	// �������� �� �������� ����������
	ExtractFileExtension(FileName,Ext);
	// ���������� � ��������� �������������� �������
	if (Ext=="TGA") return LoadTGAFile(FileName);
	if (Ext=="JPG") return LoadJPGFile(FileName);
	
	// ����������� ����������, ��������� False
	return FALSE;
}

// ����������� ��������� ��� �������� TGA
const int TARGA_NO_COLORMAP = 0;
const int TARGA_COLORMAP = 1;
const int TARGA_EMPTY_IMAGE = 0;
const int TARGA_INDEXED_IMAGE = 1;
const int TARGA_TRUECOLOR_IMAGE = 2;
const int TARGA_BW_IMAGE = 3;
const int TARGA_INDEXED_RLE_IMAGE = 9;
const int TARGA_TRUECOLOR_RLE_IMAGE = 10;
const int TARGA_BW_RLE_IMAGE = 11;

// ��������� ��������� TGA �����
struct TTargaHeader
{
    unsigned __int8 IDLength;
	unsigned __int8 ColorMapType;
	unsigned __int8 ImageType;
    unsigned __int16 ColorMapOrigin;
	unsigned __int16 ColorMapSize;
    unsigned __int8 ColorMapEntrySize;
    unsigned __int16 XOrigin;
	unsigned __int16 YOrigin;
	unsigned __int16 Width;
	unsigned __int16 Height;
    unsigned __int8 PixelSize;
    unsigned __int8 ImageDescriptor;
};


// ������� �������� TGA �����
BOOL gunTexture::LoadTGAFile(const gunString &FileName)
{
	BYTE   		header[18];									// ����� ��� ��������� TGA �����
	UINT  		ImageSize;									// ������ ����������� � ������
	TTargaHeader Header;									// ��������� TGA

	BYTE *RLEBuffer,*ImageData;								// ������� ��� ���������� TGA
	UINT I,LineSize,bpp8;									// ��������� ����������
	BOOL FlipV;												// ������������ ����������?
	size_t RLECount=0,RLESize=0;							// ��� ����������

	FILE *file = fopen(FileName.data(), "rb");				// ��������� TGA ����
    if (!file)												// ���� ��� ������ �����
	{
		// ��������� False
		return FALSE;
	}
	// ��������� ���� � �������� ����� � ���������
	TextureName=FileName;

	// ������ ��������� �� �����
	fread(&header,1,sizeof(header),file);

	// ��������� ��������� �������
    Header.IDLength=header[0];
	Header.ColorMapType=header[1];
	Header.ImageType=header[2];
    Header.ColorMapOrigin=header[3]+256*header[4];
	Header.ColorMapSize=header[5]+256*header[6];
    Header.ColorMapEntrySize=header[7];
    Header.XOrigin=header[8]+256*header[9];
	Header.YOrigin=header[10]+256*header[11];;
	Header.Width=header[12]+256*header[13];
	Header.Height=header[14]+256*header[15];
    Header.PixelSize=header[16];
    Header.ImageDescriptor=header[17];

	// ���� �� ������������ �����������
	FlipV = ((Header.ImageDescriptor & 0x20)!=0);
	Header.ImageDescriptor = Header.ImageDescriptor & 0x0F;

	// ��� �� �������
	Bpp=Header.PixelSize;
	// ���� �� �������
	bpp8=Bpp/8;
	// ������
	Width = Header.Width;
	// ������
	Height = Header.Height;
	// ������ �����
    LineSize = Width * (bpp8);
	// ������ ����������� � ������
	ImageSize=Width*Height*(bpp8);
	// �������� ������ ��� ��������
	Data.resize(ImageSize);
	// �������� ������ ��� ������������� �����������
	ImageData=new BYTE[ImageSize];
	// �������� ������ ��������
	switch (Header.ImageType)
	{
	case TARGA_EMPTY_IMAGE:;
	case TARGA_BW_IMAGE:;
	case TARGA_INDEXED_IMAGE: break;
	case TARGA_TRUECOLOR_IMAGE: // ������������� �����������
		// ������ ��������� �� �����
		fread(ImageData,1,ImageSize,file);
		break;
	case TARGA_BW_RLE_IMAGE:;
    case TARGA_INDEXED_RLE_IMAGE:;
    case TARGA_TRUECOLOR_RLE_IMAGE: // ����������� RLE 
		// ������� � �����
		fpos_t cpos,flen;
		// ���������� �������
		fgetpos(file,&cpos);
		// ���� �� ����� �����
		fseek(file,0,SEEK_END);
		// ���������� �������
		fgetpos(file,&flen);
		// ����������� ��� ��� ����
		fseek(file,(long)cpos,SEEK_SET);
		// ������ ������ ������ ��� ������� ����� ���������
		RLESize=(UINT)(flen-cpos)+1;
		// �������� ����� ��� ������ ������
		RLEBuffer = new BYTE[RLESize];
		// ������ ��
		RLECount=fread(RLEBuffer,(size_t)1,(size_t)flen+1,file);
		// ���������� ��� ����������
        UINT count=0,rcount=1,rloop=0,pakleft=0;
		// �������� �� ���������� �����
		pakleft=1+RLEBuffer[rcount-1];
		// ���� �� �� �����������
		while (count<(Width*Height-1)*bpp8)
		{
			// ���� �� ����� �������� ���� ����
			if (pakleft==0)
			{
				// ������ ����
				if (rcount<RLESize)
				while ((RLEBuffer[rcount] > 0x7F)&&(count<(Width*Height-1)*bpp8)&& (rcount<RLESize))
				{
					// ������ �����
					rloop = 1 + (RLEBuffer[rcount] & 0x7F);
					// ���� ������ ������ ��� ����
					if ((rloop*bpp8+count)>=(Width*Height-1)*bpp8)
					{
						// ��������
						rloop=(Width*Height-1)-count/bpp8;
					}
					// ����������� �������
					rcount++;
					// ���� ��� �����������
					for (I=0;I<rloop;I++)
					{
						// ���������� ����
						memcpy(ImageData+count,RLEBuffer+rcount,bpp8);					
						count+=bpp8; // ������� ������� �� �������
					}
					rcount+=bpp8; // ����������� ������� �� �������
					if (count>=bpp8*Width*Height-1) //���� �� ��� ���������
						break; //�������
				}
				//�� ���������� ����� ��������
				pakleft=1+RLEBuffer[rcount];
				rcount++; // ����������� �������
			}
			else //����� ���� ������������
			{
				// ������ �������� ������
				memcpy(ImageData+count,RLEBuffer+rcount,bpp8*pakleft);					
				// ������� �� ������
				rcount+=bpp8*pakleft;
				// ������� �� ������
				count+=bpp8*pakleft;
				// �������� ����
				pakleft=0;
			} // ����� if (pakleft==0) .....
		} // ����� while
        delete RLEBuffer;  // ������� ������ ������ ������
		break; // ������� �� switch
	}

	// �������� � �������� � ����������� �� � �������
	switch (Bpp)
	{
	case 24: // 24 ���� �� �������
		{
			// ���� � ImageData ��������� � ������� BGR
			// � ��� ���� RGB
			// ����������:
			gunColor3ub *j;
			j=(gunColor3ub *)&Data[0];
			for(UINT i=0; i<ImageSize; i+=bpp8)	
			{									
				if (FlipV)
				{
					j->RGBColor.b = ImageData[i];	
					j->RGBColor.g = ImageData[i + 1];
					j->RGBColor.r = ImageData[i + 2];
				}
				else
				{
					j->RGBColor.b = ImageData[(Height-i/LineSize-1)*LineSize+i%LineSize + 0];	
					j->RGBColor.g = ImageData[(Height-i/LineSize-1)*LineSize+i%LineSize + 1];
					j->RGBColor.r = ImageData[(Height-i/LineSize-1)*LineSize+i%LineSize + 2];
				}
				j++;
			}
			break; // ������� �� switch
		}
	case 32: // 32 ���� �� �������
		{
			// ���� � ImageData ��������� � ������� BGRA
			// � ��� ���� RGBA
			// ����������:
			gunColor4ub *j;
			j=(gunColor4ub *)&Data[0];
			for(UINT i=0; i<ImageSize; i+=bpp8)	
			{									
				j->RGBColor.b = ImageData[i];	
				j->RGBColor.g = ImageData[i + 1];
				j->RGBColor.r = ImageData[i + 2];
				j->RGBColor.a = ImageData[i + 3];
				j++;
			}
			break; // ������� �� switch
		}
	}

	delete ImageData; // ������� ��������� ������

	return TRUE;	// ��������� True
}

// �������� �������� �� ����� JPG
BOOL gunTexture::LoadJPGFile(const gunString &FileName)
{
	// ���������� ���� ��� �������� ��������
	// ��������� ����������
	BYTE Bpp8;		// ���� �� �������
	int RetVal;		// ����������� IJL �������� 
	// ��������� jpg ��������
	JPEG_CORE_PROPERTIES image;
	// ����������� ��� ��� ������
	ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	// �������������� ����������
	if( ijlInit( &image ) != IJL_OK )
	{
		// ��������, ��������� False
		return FALSE;
	}
	// ��� ����� ��������
	image.JPGFile = FileName.data();
	// ��������� �������� � ������
	if ((RetVal = ijlRead(&image,IJL_JFILE_READPARAMS)) == IJL_OK)
	{
		// ��������� ������
		Height = image.JPGHeight;
		// ��������� ������
		Width = image.JPGWidth;
		// ���� �� �������
		Bpp8 = 3;
		// ��� �� �������
		Bpp = Bpp8*8;
		// ������ ��������
		UINT ImageSize=Height*Width*Bpp8;
		// �������� ������ ��� ��������
		Data.resize(ImageSize);
		// ������� ������ �����������
		BYTE *img = new BYTE[ImageSize];
		// ���� ������ ��������
		if (img)
		{
			// ��������� ������ � �������
			image.DIBBytes = img;
			// ������ RGB
			image.DIBColor = IJL_RGB;
			// ������
			image.DIBHeight = Height;
			// ������
			image.DIBWidth = Width;
			// ���� �� �������
			image.DIBChannels = Bpp8;

			// ���������� jpg ���� � ������ ��������
			if ((RetVal = ijlRead(&image,IJL_JFILE_READWHOLEIMAGE)) == IJL_OK)
			{
				// �� � ������ ��� ����
				// �������� ������ ��������
				switch (Bpp)
				{
				case 24:  // 24 ���� �� �������
                    {
						// ������ �������� � ��������
						gunColor3ub *j;
						j=(gunColor3ub *)&Data[0];
						for(UINT i=0; i<ImageSize; i+=Bpp8)	
						{									
							j->RGBColor.r = img[i + 0];	
							j->RGBColor.g = img[i + 1];
							j->RGBColor.b = img[i + 2];
							j++;
						}
						break;  // ������� �� switch
					}

				case 32:  // 32 ���� �� �������
					{
						// �������� � ��������, �������� �����-�����
						gunColor4ub *j;
						j=(gunColor4ub *)&Data[0];
						for(UINT i=0; i<ImageSize; i+=Bpp8)	
						{									
							j->RGBColor.r = img[i + 0];	
							j->RGBColor.g = img[i + 1];
							j->RGBColor.b = img[i + 2];
							j++;
						}
						break; // ������� �� switch
					}
				}
		 }
		}
		delete img;  // ������� ��������� ������
	}
	ijlFree(&image); // ����������� ������ ������

	// ��������� True, �� ������ �������
	return TRUE;
}

// ������ ������ ������ � ���� JPG
// ������������ ��� �������� ScreenShot'��
BOOL gunSaveScreenToJPG()
{
	// ��������� jpg ����
	JPEG_CORE_PROPERTIES image;
	// ����������� ��� ���� ������
    ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );

	// �������������� IJL 
	if( ijlInit( &image ) != IJL_OK ) return FALSE; //�������, ���� �� ����������

	// �������� ����� ��� ����������� ������*������*����_��_�������
	BYTE *Data=new BYTE[MainSettings.SCREEN_WIDTH*MainSettings.SCREEN_HEIGHT*3];
	// �������� ����������� � ������
	gunGetScreen(Data);
	// ����������� ������������, �� ���������
	UINT LineLen=MainSettings.SCREEN_WIDTH*3; // ������ �����
	BYTE *c=new BYTE[LineLen]; // ��� ����������� �����
	for (UINT i=0;i<MainSettings.SCREEN_HEIGHT/2;i++)
	{
		memcpy(c,Data+i*LineLen,LineLen);
		memcpy(Data+i*LineLen,Data+(MainSettings.SCREEN_HEIGHT-i-1)*LineLen,LineLen);
		memcpy(Data+(MainSettings.SCREEN_HEIGHT-i-1)*LineLen,c,LineLen);
	}
	delete c;


	gunString FileName; // ��� �����

	FileName.resize(15);	// ������ �������� ����� = 15
	char *da=&FileName[0];  // ������ �� ��������
	
	// ���� ��� ���������� ���������� �����
	for (UINT i=0;i<1000;i++)
	{
		// ������ ��������
		// �������� Screen000.jpg, Screen001.jpg, � �.�.
		sprintf(da,"Screen%03d.jpg",i);
        FILE *file=fopen(da,"r");  // ��������� ����
		if (!file) // ���� ������ ����� ����
		{
			// �����, �������
			break;
		}
		// ��������� ����, ���� ���������
		fclose(file);
	}

	
	// ������������� DIB
	image.DIBWidth         = MainSettings.SCREEN_WIDTH;			// ������
	image.DIBHeight        = MainSettings.SCREEN_HEIGHT;		// ������
	image.DIBBytes         = Data;								// ������ ��� ������

	// ������������� ������ ��� JPEG
	image.JPGFile          = da;								// �������� �����
	image.JPGWidth         = MainSettings.SCREEN_WIDTH;			// ������
	image.JPGHeight        = MainSettings.SCREEN_HEIGHT;		// ������

	image.DIBColor       = IJL_RGB;								// ������ BGR
	image.DIBChannels    = 3;									// ���� �� �������
	image.DIBPadBytes    = IJL_DIB_PAD_BYTES(image.DIBWidth,3);	// ������� �����
	image.JPGColor       = IJL_YCBCR;							// ������ �����
	image.JPGChannels    = 3;									// ���� �� �������
	image.JPGSubsampling = IJL_NONE;								// ��� ������������

	// ���������� �����������
	if( ijlWrite( &image, IJL_JFILE_WRITEWHOLEIMAGE ) != IJL_OK ) return FALSE; // �������, ���� �� ����������
	// ����������� ������
	if( ijlFree( &image ) != IJL_OK ) return FALSE; // �������, ���� �� ����������

	delete Data; // ����������� ������
	
	// ��������� True, �� ������ �������
	return TRUE;
}

// ������ ��������
UINT gunNewTexture()
{
	Texture.push_back(gunTexture());
	return (UINT)Texture.size()-1;
}

// ��������� ������� �������� � ������� �������
int gunCheckTexture(const gunString& FileName)
{
	for(IT(gunTexture) it=Texture.begin();it<Texture.end();it++)
		if (it->TextureName==FileName)
			return (UINT)(it-Texture.begin());
	return -1;

}