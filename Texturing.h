#pragma once
#include "D:\\������\freeglut-2.8.1\include\GL\freeglut.h"
#include "DefineTexture.h"

  void BindingTexture(Texture *texture)
 {

	//glGenTextures(1, &texture->texID); // ������� � ������������� ���� 100 ��� �� ����������� ��������!!!
    glBindTexture(GL_TEXTURE_2D, texture->texID);

    gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp / 8,  texture->width,  texture->height, texture->type, GL_UNSIGNED_BYTE,  texture->imageData);		
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

/* GL_REPLACE  GL_KEEP  GL_DECAL */

 }

    void BindingText(Texture *texture)
 {

	//glGenTextures(1, &texture->texID); // ������� � ������������� ���� 100 ��� �� ����������� ��������!!!
    glBindTexture(GL_TEXTURE_2D, texture->texID);

 }

 // ��� ���� ����� �������: ������ ������ �������� ��� ������� � ��
void CreateRenderTexture(Texture textureArray[], int size, int channels, int type, int textureID)
{
    // ��������� ��� ���������� ������ ��������
    unsigned int *pTexture = NULL;

    // ����� ������� ������ �������� ��� ������� ������������ ��������.
    // ����� ������� ���, ������ ������� ������ ��� �������� ������ �
    // �������� ��� OpenGL. �������� �������� � ����������, ��� ��� �� �����
    // ���������� ������ � ����� ������.
    // ��� ������� ��������� ���������� ������ ��� ���������� ��������,
    // ������ ��������, ������ � ������, ������ (1,3,4), ��� (RGB, RGBA, etc)
    // � ���������� ID.

    // �������������� ������ ��� ������ �������� � �������� � pTexture
    pTexture = new unsigned int [size * size * channels];
    memset(pTexture, 0, size * size * channels * sizeof(unsigned int));

    // �������������� �������� � OpenGL � �������� � ID
    glGenTextures(1, &textureArray[textureID].texID);
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID].texID);

    // ������� �������� � ��������� � ������
    glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, pTexture);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp / 8,  texture->width,  texture->height, texture->type, GL_UNSIGNED_BYTE,  texture->imageData);

    // ������������� ��������
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // �������� �������� � ���. OpenGL, �� ����� ������� ��������� ������
    delete [] pTexture;
}