#include "gunfont.h"
#include "GunRender.h"

// ���������� ������ �������
std::vector<gunFont> Font;

// �������� ������ ������, ��������� ������
UINT gunNewFont()
{
	// ����������� ������, ��������� � �����
	Font.push_back(gunFont());
	// ��������� ������ ���������� ��������
	return (UINT)Font.size()-1;
}

// ���������� ������
gunFont::~gunFont(void)
{
}

//��������� �������� � ��������� ������� ��������� ����� �������
BOOL gunFont::LoadTextureAndCheckSpace(const gunString &FileName,const gunString &FontName,BYTE Zero)
{
	// ���������, �������� ����� ��� ���� ��������� � ������
	if (TextureID=gunCheckTexture(FileName)==-1)
	{
		// ����� ������ ����� �������� � �������
		TextureID=gunNewTexture();
		// ���� �������� �� �������� �� �����, �� �������
		if (!Texture[TextureID].LoadFromFile(FileName)) return FALSE;
		// ���������� �������� � �����-������
		MainRender.TextureUpload(Texture[TextureID]);
		// ��� ������
		Name=FontName;
	}

	// ������ �� ��������
	gunTexture *tex=&Texture[TextureID];
	// ������ �� �������� � ��������
	gunColor3ub *col=(gunColor3ub *)&tex->Data[0];

	// ���� �� ���� ������, ��� ������� ����� x+y*16
	for (int y=0;y<16;y++)
		for (int x=0;x<16;x++)
		{
			// �������:
			// 1. ��������� ������� 16�16, � ������� ������ ���� �����.
			// 2. ���� ������ ������������ ����� ���������� ����� ����
			//	  ��������� ������, ������� ����� � ����� ������.
			// 3. �������� FTrim - ������� ��������� ����� �������.

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

	// ����������� �������
	return TRUE;
}

// ������� ������ ������ �� �����
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