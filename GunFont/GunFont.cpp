#include "gunfont.h"
#include "GunRender.h"

// Глобальный массив шрифтов
std::vector<gunFont> Font;

// Создание нового шрифта, возращает индекс
UINT gunNewFont()
{
	// Увеличиваем массив, добавляем в конец
	Font.push_back(gunFont());
	// Возращаем индекс последнего элемента
	return (UINT)Font.size()-1;
}

// Диструктор пустой
gunFont::~gunFont(void)
{
}

//Загружаем текстуру и заполняем таблицу растояний между буквами
BOOL gunFont::LoadTextureAndCheckSpace(const gunString &FileName,const gunString &FontName,BYTE Zero)
{
	// Проверяем, текстура может уже быть загружена в массив
	if (TextureID=gunCheckTexture(FileName)==-1)
	{
		// Иначе делаем новую текстуру в массиве
		TextureID=gunNewTexture();
		// Если текстура не грузится из файла, то выходим
		if (!Texture[TextureID].LoadFromFile(FileName)) return FALSE;
		// Закачиваем текстуру в видео-память
		MainRender.TextureUpload(Texture[TextureID]);
		// Имя шрифта
		Name=FontName;
	}

	// Ссылка на текстуру
	gunTexture *tex=&Texture[TextureID];
	// Ссылка на текстель в текстуре
	gunColor3ub *col=(gunColor3ub *)&tex->Data[0];

	// Цикл по всем буквам, код символа равен x+y*16
	for (int y=0;y<16;y++)
		for (int x=0;x<16;x++)
		{
			// Принцип:
			// 1. Вычисляем квадрат 16х16, в котором должна быть буква.
			// 2. Ищем первую вертикальную линию содержащую точку выше
			//	  заданного порога, начиная слева и потом справа.
			// 3. Получаем FTrim - таблицу растояний между буквами.

			int cx,cy;
			cy=y*16;
			cx=x*16-1;
			BOOL space=TRUE;
			while (space)
			{
				cx++;
				for (int t=0;t<16;t++)
					if ((col[cx+(cy)*16+t*256].RGBColor.r>Zero)
						||
						(col[cx+(cy)*16+t*256].RGBColor.g>Zero)
						||
						col[cx+(cy)*16+t*256].RGBColor.b>Zero)
					{
						space=FALSE;
						break;
					}
					
			}
			FTrim[x+y*16][0]=cx-x*16;
			cy=y*16;
			cx=(x+1)*16;
			space=TRUE;
			while (space)
			{
				cx--;
				for (int t=0;t<16;t++)
					if ((col[cx+(cy)*16+t*256].RGBColor.r>Zero)
						||
						(col[cx+(cy)*16+t*256].RGBColor.g>Zero)
						||
						col[cx+(cy)*16+t*256].RGBColor.b>Zero)
					{
						space=FALSE;
						break;
					}
				
			}
			FTrim[x+y*16][1]=cx+1-FTrim[x+y*16][0]-x*16;
		}

	// Возращаемся успешно
	return TRUE;
}

// Функция выбора шрифта по имени
BOOL gunSelectFont(const gunString &Name)
{
	for (IT(gunFont) it=Font.begin();it<Font.end();it++)
		if (it->Name==Name) 
		{
			MainRender.CurFont=(UINT)(it-Font.begin());
			return TRUE;
		}
	return FALSE;
}