/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/
#include "GunTexture.h"
#include "GunRender.h"

// Библиотека Intel JPEG library, для работы с jpg файлами
#include "IJL\ijl.h"
#pragma comment( lib, "src\\IJL\\ijl15.lib" )

std::vector<gunTexture> Texture;

gunTexture::~gunTexture(void)
{
}

//Создаёт текстуру заполненую цветом в 3 компоненты
void gunTexture::CreateEmpty(int width,int height, gunColor3ub &c)
{
	// Сохраняем ширину и высоту
	Width=width;
	Height=height;
	// Бит на пиксель
    Bpp=sizeof(gunColor3ub)*8;
	// Выделяем место под текстуру
	Data.resize(width*height*sizeof(gunColor3ub));
	// Заполняем заданным цветом
	for (std::vector<BYTE>::iterator i=Data.begin();i<Data.end();i+=sizeof(gunColor3ub))
		memcpy(&*i,&c,sizeof(gunColor3ub));

}

void gunTexture::CreateEmpty(int width,int height, gunColor4ub &c)
{
	// Сохраняем ширину и высоту
	Width=width;
	Height=height;
	// Бит на пиксель
	Bpp=sizeof(gunColor3ub)*8;
	// Выделяем место под текстуру
	Data.resize(width*height*sizeof(gunColor4ub));
	// Заполняем заданным цветом
	for (std::vector<BYTE>::iterator i=Data.begin();i<Data.end();i+=sizeof(gunColor4ub))
		memcpy(&*i,&c,sizeof(gunColor4ub));
}

//Вырезаем из названия файла его расширение
void ExtractFileExtension(const gunString &FileName,gunString &Ext)
{
	size_t i=FileName.rfind('.',FileName.size()-1);
	Ext=FileName.substr(i+1,FileName.size()-i-1);
	_strupr((char *)Ext.c_str());

}

// Загружаем текстуру из файла
BOOL gunTexture::LoadFromFile(const gunString &FileName)
{
	gunString Ext;
	// Вырезаем из названия расширение
	ExtractFileExtension(FileName,Ext);
	// Сравниваем и запускаем соответсвующую функцию
	if (Ext=="TGA") return LoadTGAFile(FileName);
	if (Ext=="JPG") return LoadJPGFile(FileName);
	
	// Неизвестное расширение, возращаем False
	return FALSE;
}

// Необходимые константы для загрузки TGA
const int TARGA_NO_COLORMAP = 0;
const int TARGA_COLORMAP = 1;
const int TARGA_EMPTY_IMAGE = 0;
const int TARGA_INDEXED_IMAGE = 1;
const int TARGA_TRUECOLOR_IMAGE = 2;
const int TARGA_BW_IMAGE = 3;
const int TARGA_INDEXED_RLE_IMAGE = 9;
const int TARGA_TRUECOLOR_RLE_IMAGE = 10;
const int TARGA_BW_RLE_IMAGE = 11;

// Структура заголовка TGA файла
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


// Функция загрузки TGA файла
BOOL gunTexture::LoadTGAFile(const gunString &FileName)
{
	BYTE   		header[18];									// Место для заголовка TGA файла
	UINT  		ImageSize;									// Размер изображения в байтах
	TTargaHeader Header;									// Заголовой TGA

	BYTE *RLEBuffer,*ImageData;								// Буффера для распаковки TGA
	UINT I,LineSize,bpp8;									// Служебные переменные
	BOOL FlipV;												// Перевернутое изоражение?
	size_t RLECount=0,RLESize=0;							// Для распаковки

	FILE *file = fopen(FileName.data(), "rb");				// Открываем TGA файл
    if (!file)												// Если нет такого файла
	{
		// Возращаем False
		return FALSE;
	}
	// Сохраняем путь и название файла с текстурой
	TextureName=FileName;

	// Читаем заголовок из файла
	fread(&header,1,sizeof(header),file);

	// Заполняем заголовок вручную
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

	// Тест на перевернутое изображение
	FlipV = ((Header.ImageDescriptor & 0x20)!=0);
	Header.ImageDescriptor = Header.ImageDescriptor & 0x0F;

	// Бит на пиксель
	Bpp=Header.PixelSize;
	// Байт на пиксель
	bpp8=Bpp/8;
	// Ширина
	Width = Header.Width;
	// Высота
	Height = Header.Height;
	// Длинна линии
    LineSize = Width * (bpp8);
	// Размер изображения в байтах
	ImageSize=Width*Height*(bpp8);
	// Выделяем память под текстуру
	Data.resize(ImageSize);
	// Выделаем память под необработаное изображение
	ImageData=new BYTE[ImageSize];
	// Выбираем способ загрузки
	switch (Header.ImageType)
	{
	case TARGA_EMPTY_IMAGE:;
	case TARGA_BW_IMAGE:;
	case TARGA_INDEXED_IMAGE: break;
	case TARGA_TRUECOLOR_IMAGE: // Незапакованое изображение
		// Читаем полностью всё сразу
		fread(ImageData,1,ImageSize,file);
		break;
	case TARGA_BW_RLE_IMAGE:;
    case TARGA_INDEXED_RLE_IMAGE:;
    case TARGA_TRUECOLOR_RLE_IMAGE: // Запакованое RLE 
		// Позиция в файле
		fpos_t cpos,flen;
		// Запонимаем позицию
		fgetpos(file,&cpos);
		// Идем на конец файла
		fseek(file,0,SEEK_END);
		// Запоминаем позицию
		fgetpos(file,&flen);
		// Возращаемся там где были
		fseek(file,(long)cpos,SEEK_SET);
		// Размер сжатых данных это разница между позициями
		RLESize=(UINT)(flen-cpos)+1;
		// Выделяем место под сжатые данные
		RLEBuffer = new BYTE[RLESize];
		// Читаем всё
		RLECount=fread(RLEBuffer,(size_t)1,(size_t)flen+1,file);
		// Переменные для распаковки
        UINT count=0,rcount=1,rloop=0,pakleft=0;
		// Осталось до следующего блока
		pakleft=1+RLEBuffer[rcount-1];
		// Пока всё не распаковано
		while (count<(Width*Height-1)*bpp8)
		{
			// Если до блока осталось ноль байт
			if (pakleft==0)
			{
				// Читаем блок
				if (rcount<RLESize)
				while ((RLEBuffer[rcount] > 0x7F)&&(count<(Width*Height-1)*bpp8)&& (rcount<RLESize))
				{
					// Длинна блока
					rloop = 1 + (RLEBuffer[rcount] & 0x7F);
					// Если длинна больше чем надо
					if ((rloop*bpp8+count)>=(Width*Height-1)*bpp8)
					{
						// Обрезаем
						rloop=(Width*Height-1)-count/bpp8;
					}
					// Увеличиваем счетчик
					rcount++;
					// цикл для размножения
					for (I=0;I<rloop;I++)
					{
						// размножаем блок
						memcpy(ImageData+count,RLEBuffer+rcount,bpp8);					
						count+=bpp8; // двигаем счетчик на пиксель
					}
					rcount+=bpp8; // Увеличиваем счетчик на пиксель
					if (count>=bpp8*Width*Height-1) //Если всё уже прочитано
						break; //выходим
				}
				//До следующего блока осталось
				pakleft=1+RLEBuffer[rcount];
				rcount++; // Увеличиваем счетчик
			}
			else //Иначе блок продолжается
			{
				// Просто копируем данные
				memcpy(ImageData+count,RLEBuffer+rcount,bpp8*pakleft);					
				// Счетчик на длинну
				rcount+=bpp8*pakleft;
				// Счетчик на длинну
				count+=bpp8*pakleft;
				// Осталось ноль
				pakleft=0;
			} // Конец if (pakleft==0) .....
		} // Конец while
        delete RLEBuffer;  // Удаляем буффер сжатых данных
		break; // выходим из switch
	}

	// Копируем в текстуру в зависимости от её формата
	switch (Bpp)
	{
	case 24: // 24 бита на пиксель
		{
			// Цвет в ImageData находится в формате BGR
			// А нам надо RGB
			// Пребразуем:
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
			break; // Выходим из switch
		}
	case 32: // 32 бита на пиксель
		{
			// Цвет в ImageData находится в формате BGRA
			// А нам надо RGBA
			// Пребразуем:
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
			break; // Выходим из switch
		}
	}

	delete ImageData; // Удаляем временные данные

	return TRUE;	// Возращаем True
}

// Загрузка текстуры из файла JPG
BOOL gunTexture::LoadJPGFile(const gunString &FileName)
{
	// Собственно весь код загрузки текстуры
	// объявляем переменные
	BYTE Bpp8;		// Байт на пиксель
	int RetVal;		// Возращаемое IJL значение 
	// объявляем jpg текстуру
	JPEG_CORE_PROPERTIES image;
	// освобождаем под нее память
	ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	// инициализируем библиотеку
	if( ijlInit( &image ) != IJL_OK )
	{
		// Неудачно, возращаем False
		return FALSE;
	}
	// имя файла текстуры
	image.JPGFile = FileName.data();
	// загружаем текстуру в память
	if ((RetVal = ijlRead(&image,IJL_JFILE_READPARAMS)) == IJL_OK)
	{
		// Сохраняем Ширину
		Height = image.JPGHeight;
		// Сохраняем Высоту
		Width = image.JPGWidth;
		// Байт на пиксель
		Bpp8 = 3;
		// Бит на пиксель
		Bpp = Bpp8*8;
		// Размер текстуры
		UINT ImageSize=Height*Width*Bpp8;
		// Выделаем память под текстуру
		Data.resize(ImageSize);
		// создаем массив изображения
		BYTE *img = new BYTE[ImageSize];
		// Если память выделена
		if (img)
		{
			// Указываем буффер с данными
			image.DIBBytes = img;
			// Формат RGB
			image.DIBColor = IJL_RGB;
			// Ширина
			image.DIBHeight = Height;
			// Высота
			image.DIBWidth = Width;
			// Байт на пиксель
			image.DIBChannels = Bpp8;

			// декодируем jpg файл в массив пикселей
			if ((RetVal = ijlRead(&image,IJL_JFILE_READWHOLEIMAGE)) == IJL_OK)
			{
				// ну а дальше все ясно
				// Выбираем формат текстуры
				switch (Bpp)
				{
				case 24:  // 24 бита на пиксель
                    {
						// Просто копируем в текстуру
						gunColor3ub *j;
						j=(gunColor3ub *)&Data[0];
						for(UINT i=0; i<ImageSize; i+=Bpp8)	
						{									
							j->RGBColor.r = img[i + 0];	
							j->RGBColor.g = img[i + 1];
							j->RGBColor.b = img[i + 2];
							j++;
						}
						break;  // Выходим из switch
					}

				case 32:  // 32 бита на пиксель
					{
						// Копируем в текстуру, сохраняя альфа-канал
						gunColor4ub *j;
						j=(gunColor4ub *)&Data[0];
						for(UINT i=0; i<ImageSize; i+=Bpp8)	
						{									
							j->RGBColor.r = img[i + 0];	
							j->RGBColor.g = img[i + 1];
							j->RGBColor.b = img[i + 2];
							j++;
						}
						break; // выходим из switch
					}
				}
		 }
		}
		delete img;  // Удаляем временный буффер
	}
	ijlFree(&image); // Освобождаем сжатые данные

	// Возращаем True, всё прошло успешно
	return TRUE;
}

// Функци записи экрана в файл JPG
// Используется для создания ScreenShot'ов
BOOL gunSaveScreenToJPG()
{
	// Объявляем jpg файл
	JPEG_CORE_PROPERTIES image;
	// Освобождаем под него память
    ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );

	// Инициализируем IJL 
	if( ijlInit( &image ) != IJL_OK ) return FALSE; //Выходим, если не получилось

	// Выделяем место под изображение Ширина*Высота*Байт_на_пиксель
	BYTE *Data=new BYTE[MainSettings.SCREEN_WIDTH*MainSettings.SCREEN_HEIGHT*3];
	// Копируем изображение с экрана
	gunGetScreen(Data);
	// Изображение перевернутое, по вертикали
	UINT LineLen=MainSettings.SCREEN_WIDTH*3; // Длинна линии
	BYTE *c=new BYTE[LineLen]; // Для перемещения цвета
	for (UINT i=0;i<MainSettings.SCREEN_HEIGHT/2;i++)
	{
		memcpy(c,Data+i*LineLen,LineLen);
		memcpy(Data+i*LineLen,Data+(MainSettings.SCREEN_HEIGHT-i-1)*LineLen,LineLen);
		memcpy(Data+(MainSettings.SCREEN_HEIGHT-i-1)*LineLen,c,LineLen);
	}
	delete c;


	gunString FileName; // Имя файла

	FileName.resize(15);	// Длинна названия файла = 15
	char *da=&FileName[0];  // Ссылка на название
	
	// Цикл для нахождения свободного файла
	for (UINT i=0;i<1000;i++)
	{
		// Создаём название
		// Получаем Screen000.jpg, Screen001.jpg, и т.д.
		sprintf(da,"Screen%03d.jpg",i);
        FILE *file=fopen(da,"r");  // Открываем файл
		if (!file) // Если такого файла нету
		{
			// Нашли, выходим
			break;
		}
		// Закрываем файл, ищем следующий
		fclose(file);
	}

	
	// Устанавливаем DIB
	image.DIBWidth         = MainSettings.SCREEN_WIDTH;			// Ширина
	image.DIBHeight        = MainSettings.SCREEN_HEIGHT;		// Высота
	image.DIBBytes         = Data;								// Буффер для записи

	// Устанавливаем данные для JPEG
	image.JPGFile          = da;								// Название файла
	image.JPGWidth         = MainSettings.SCREEN_WIDTH;			// Ширина
	image.JPGHeight        = MainSettings.SCREEN_HEIGHT;		// Высота

	image.DIBColor       = IJL_RGB;								// Формат BGR
	image.DIBChannels    = 3;									// Байт на пиксель
	image.DIBPadBytes    = IJL_DIB_PAD_BYTES(image.DIBWidth,3);	// Падывые байты
	image.JPGColor       = IJL_YCBCR;							// Формат цвета
	image.JPGChannels    = 3;									// Байт на пиксель
	image.JPGSubsampling = IJL_NONE;								// Тип субсамплинга

	// Записываем изображение
	if( ijlWrite( &image, IJL_JFILE_WRITEWHOLEIMAGE ) != IJL_OK ) return FALSE; // Выходим, если не получилось
	// Освобождаем память
	if( ijlFree( &image ) != IJL_OK ) return FALSE; // Выходим, если не получилось

	delete Data; // Освобождаем буффер
	
	// Возращаем True, Всё прошло успешно
	return TRUE;
}

// Создаём текстуру
UINT gunNewTexture()
{
	Texture.push_back(gunTexture());
	return (UINT)Texture.size()-1;
}

// Проверяем наличие текстуры в массиве текстур
int gunCheckTexture(const gunString& FileName)
{
	for(IT(gunTexture) it=Texture.begin();it<Texture.end();it++)
		if (it->TextureName==FileName)
			return (UINT)(it-Texture.begin());
	return -1;

}