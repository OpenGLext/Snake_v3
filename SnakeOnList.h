#pragma once
#include "StructData.h"


class SnakeOnList
{
public:
	SnakeOnList(void);
	~SnakeOnList(void);

	int& AddElement(int& idTexture, int& glrgb, int& w,int& h, int& format, int& type, void *data);

	

	typedef std::list<int> publicItem;
	publicItem *pItem_;

private:

	typedef std::list<int> item;
	item *pItem;
    
};

