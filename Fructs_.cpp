#include "Fructs_.h"
using namespace Fruct;

Fructs_::Fructs_():m_N(30),m_M(20),m_scale(25),isActive(true)
{

	this->New(); 
	frame = 1.0f;
	depthFruct = 1.0f;

	frame = 1.0f;
	depthFruct = 1.0f;
}
	


void Fructs_::SetDepthFruct(const float depth)
{
	depthFruct = depth;
}

float Fructs_::GetDepthFruct() const
{
	return this->depthFruct;
}



Fructs_::~Fructs_()
{
	
}

Fructs_::Fructs_(int& N, int& M, int& Scale_, int& startX, int& startY)
{
	m_M = M; m_N = N; m_scale = Scale_;
	/*m_vec.x = startX; m_vec.y = startY;*/
}

Fructs_::Fructs_(const Fructs_& copyConstructor)
{
	vec *pVec = new vec;
}

void Fructs_::SetLive(const bool& active)
{
	isActive = active;
}

Fructs_& Fructs_::GetXY()
{
	return *this;
}

const bool Fructs_::GetActive()
{
	return isActive;
}



void Fructs_::New()
{
	//srand(time(0));
	srand(GetTickCount());

	for(int i=0;i<10;i++)
	{

        m_vec[i].x = (rand() %  m_N); //- sizeWall ;
		
		m_vec[i].y = (rand() %  m_M ); //- sizeWall;

       /* if ( m_vec[i].x == 0 ) { m_vec[i].x = 1;}
		if ( m_vec[i].y == 0 ) { m_vec[i].y = 1;}*/

	
         
		 if (countRun == 0)
		{
			cout<<"Fruct CountRun "<<countRun<<endl;
		  visibleFructs[i] = true;
		}
	}
}

bool Fructs_::GetVisible(const int& indx) const
{
	return visibleFructs[indx];
}

void Fructs_::SetVisible(const bool& set,const int& indx)
{
	visibleFructs[indx] = set;
}

void Fructs_::StartDraw()
{
	this->DrawApple();
}

	inline void Fructs_::box_(float x1, float y1, float x2, float y2)
	{

			glBegin(GL_QUADS);	
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x1,y1,depthFruct);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x2,y1,depthFruct);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x2,y2,depthFruct);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x1,y2,depthFruct);
	glEnd();
	}

void Fructs_::DrawApple()
{
   glColor3f(1.0,0,0);
   countRun++;

   //glRectf( m_vec[0].x * m_scale, m_vec[0].y * m_scale, (m_vec[0].x+1) * m_scale, (m_vec[0].y+1) * m_scale );

   for(int i=0;i<10;i++)
   {
	   if (visibleFructs[i] )
	   {
		   this->box_(m_vec[i].x * m_scale, m_vec[i].y * m_scale, (m_vec[i].x + frame) * m_scale, (m_vec[i].y + frame) * m_scale);
  // glRectf( m_vec[i].x * m_scale, m_vec[i].y * m_scale, (m_vec[i].x+1) * m_scale, (m_vec[i].y+1) * m_scale );
  
       }

   }
   
}


