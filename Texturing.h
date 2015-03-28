#pragma once
#include "D:\\ƒвижки\freeglut-2.8.1\include\GL\freeglut.h"
#include "DefineTexture.h"

  void BindingTexture(Texture *texture)
 {

	//glGenTextures(1, &texture->texID); // вынести в инициализацию дабы 100 раз не создавалась текстура!!!
    glBindTexture(GL_TEXTURE_2D, texture->texID);

    gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp / 8,  texture->width,  texture->height, texture->type, GL_UNSIGNED_BYTE,  texture->imageData);		
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

/* GL_REPLACE  GL_KEEP  GL_DECAL */

 }

    void BindingText(Texture *texture)
 {

	//glGenTextures(1, &texture->texID); // вынести в инициализацию дабы 100 раз не создавалась текстура!!!
    glBindTexture(GL_TEXTURE_2D, texture->texID);

 }

 // ≈щЄ одна нова€ функци€: создаЄт пустую текстуру дл€ рендера в неЄ
void CreateRenderTexture(Texture textureArray[], int size, int channels, int type, int textureID)
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
    glGenTextures(1, &textureArray[textureID].texID);
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID].texID);

    // —оздаем текстуру и сохран€ем в пам€ти
    glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, pTexture);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp / 8,  texture->width,  texture->height, texture->type, GL_UNSIGNED_BYTE,  texture->imageData);

    // ”станавливаем качество
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // —охранив текстуру с пом. OpenGL, мы можем удалить временные данные
    delete [] pTexture;
}