class ListBox;

#ifndef DEF_LIST_BOX
#define DEF_LIST_BOX

#include "../c_Window.h"

class ListBox : public Window{
public:
	ListBox(int x,int y,int w,int h,Window *_parent,int _option,int _id=UNIQUE_ID);
	virtual ~ListBox();
	int addString(char *str);
	void setCur(int index);
	int getCur();
};





#endif
