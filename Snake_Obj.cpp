#include "Snake_Obj.h"
using namespace SNAKE;

Snake_Obj::Snake_Obj():m_scale(25),m_dir(1),m_len_body(4),m_IsCollision(false),firstRun(true)
{
		s[0].x = 10; s[0].y = 10;
		GetArrayXY();
		textid = 11;
		gravity = 0.0f;
		GameOver = false;
		speedsnake = 1.0f;
		frame = 0.9f;
		headTexture_ *headTexture = new headTexture_;
	    Quad **pQuad = new Quad*;
		//pQuad->LoadTexture(headTexture,"head.tga");
		Texture *texture = new Texture;	
		//pQuad->LoadTexture(texture,"head.tga");
		speedsnake = 1.0f;
		frame = 0.9f;
		timeDraw = 1.0f;
}

bool Snake_Obj::IsGameOver() const
{
	return GameOver;
}

bool Snake_Obj::IsCollision() const
{
	return m_IsCollision;
}

Snake_Obj::~Snake_Obj()
{
	 delete PointFruct;
	 PointFruct = NULL;
	
	 delete headTexture;
	
}

const int& Snake_Obj::GetLenBody() const
{
	return m_len_body;
}

void Snake_Obj::Tick()
{
    

	for(int i=m_len_body; i>0; --i)
	{
		s[i].x = s[i-1].x;
		s[i].y = s[i-1].y;
	}

	if (m_dir == 0 ) s[0].y += 1;
	if (m_dir == 1 ) s[0].x -= 1;
	if (m_dir == 2 ) s[0].x += 1;
	if (m_dir == 3 ) s[0].y -= 1;

	std::cout<<"Head y"<<s[0].y<<endl;

	if (this->IsHeadIsTail()) { GameOver = true; }
	if (this->IsOutWall())    { GameOver = true; }
    else GameOver = false;
	this->Collision();
	

	
	
}

void Snake_Obj::SetLenghtBody(const int& len)
{
	this->m_len_body = len;
}

bool Snake_Obj::IsHeadIsTail()
{
	for (int i=1; i<m_len_body; i++)
		if ( ( s[0].x == s[i].x ) && ( s[0].y == s[i].y) )
		{
			//this->SetLenghtBody(this->GetLenBody()-1); 
                             // или уменьшим на -1 тело

			// уменьшим жизнь 
			this->SetLenghtBody(4); 
			std::cout<<"---Len "<<endl;
			return true; // уменьшим жизнь 
		}
}

 inline void Snake_Obj::box_(float x1, float y1, float x2, float y2)
{
	glBegin(GL_QUADS);	
			glTexCoord2f(0.0f, 0.0f); glVertex2f(x1,y1);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(x2,y1);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(x2,y2);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(x1,y2);
	glEnd();
 }



 



 void Snake_Obj::BindingTexture(headTexture_ *headTexture)
 {

	 glEnable(GL_TEXTURE_2D); 
	// glGenTextures(1, (&headTexture)->texID);
    glBindTexture(GL_TEXTURE_2D, texture->texID);

gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp / 8,  texture->width,  texture->height, texture->type, GL_UNSIGNED_BYTE,  texture->imageData);		
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

/* GL_REPLACE  GL_KEEP  GL_DECAL */

 
 }

 void Snake_Obj::SetTimeDraw(const float timeDraw_)
 {
	 timeDraw = timeDraw_;
 }


void Snake_Obj::DrawSnake()
	{
		

		glColor3f(0,1.0,0);
		for(int i=0; i<m_len_body; i++)
		{

		//	this->box_(s[i].x *m_scale * timerx->GetElapsedSec(), s[i].y * m_scale * timerx->GetElapsedSec(),
		//		      (s[i].x+0.9)*m_scale * timerx->GetElapsedSec(), (s[i].y+0.9)*m_scale * timerx->GetElapsedSec() );

		
			
			this->box_(s[i].x * m_scale *timeDraw ,
				       s[i].y * m_scale *timeDraw ,
				      (s[i].x + frame) * m_scale *timeDraw , 
					  (s[i].y + frame) * m_scale *timeDraw);
			
			

		}
	}

bool Snake_Obj::IsFirstRun() const
{
	return firstRun;
}

void Snake_Obj::SetDir(const int dir) 
	{
		m_dir = dir;

	}	

void Snake_Obj::Draw()
	{
		this->Tick();
		DrawSnake();
	}

void Snake_Obj::SetBorder(const int& Left, const int& Right, const int& Top, const int& Bottom)
	{
		left = Left; right = Right; top = Top; bottom = Bottom;
		
	}

const bool Snake_Obj::IsOutWall()
	{

		if ( s[0].x < (0 ) ) 
		{
			m_dir = 2;

			return true;
		}

		if ( s[0].x > 27 )
		{
			m_dir = 1;
			return true;
		}

		if ( s[0].y > 17 ) //
		{
			m_dir = 3; //2
			return true;
		}

		if ( s[0].y < 1 )
		{
			m_dir = 0;
			return true;
		}

		return false;
	}

bool Snake_Obj::IsCollisionWithBlock(const int& xBlock, const int& yBlock)
{

	if ( ( s[0].x == xBlock ) && ( s[0].y == yBlock) ) { this->SetLenghtBody(4); return true;}
	                                                     // уменьшим тело до стартового
	return false;
}

void Snake_Obj::GetArrayXY()
{
	Fruct::Fructs_ *PointFruct = new Fruct::Fructs_; 
	PointFruct->GetXY();
		
if ( PointFruct == NULL ) 
{ 
	cout<<"PointFruct = NULL!!!"<<endl;
}
else 
{
	for(int i=0;i<10;i++)
	{
		m_fruct_x[i] = PointFruct->m_vec[i].x;
		m_fruct_y[i] = PointFruct->m_vec[i].y;
	}

}

}

void Snake_Obj::SetCollision(const bool SET)
{
	this->m_IsCollision = SET;
}

 const bool Snake_Obj::Collision()
{
	Fruct::Fructs_ *PointFruct = new Fruct::Fructs_; 	
	PointFruct->GetXY();
		
if ( PointFruct == NULL ) 
{ 
	cout<<"PointFruct = NULL!!!"<<endl;
}
   
	  
            for(int i=0;i<10;i++)
				  
			if ((s[0].x == m_fruct_x[i]) && (s[0].y) == m_fruct_y[i])
	            {
	        m_len_body++;
		    m_IsCollision = true;
		//	firstRun = false;
			PointFruct->SetVisible(false,i);
			bool t = PointFruct->GetVisible(i);
			//cout<<"VisibleFructs[i] "<<i<<" "<<t<<endl;
				 // сотрем фрукт
				/* glColor3f(0,0,0);
	             glRectf(m_fruct_x[i],m_fruct_y[i],m_fruct_x[i]*25,m_fruct_y[i]*25);

				 glutSwapBuffers();*/
			
			   }
	        return true;                 
	
      return false;   
}

	

	
