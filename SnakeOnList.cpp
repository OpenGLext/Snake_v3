#include "SnakeOnList.h"


SnakeOnList::SnakeOnList(void)
{
	item *pItem = new item;
	publicItem *pItem_ = new publicItem;

}


SnakeOnList::~SnakeOnList(void)
{
	delete pItem; delete pItem_;
}



int& SnakeOnList::AddElement(int& idTexture, int& glrgb, int& w,int& h, int& format, int& type, void *data) 
   
{
	pItem_->push_back(idTexture);
	int sz = pItem_->size();
	return sz;
}
