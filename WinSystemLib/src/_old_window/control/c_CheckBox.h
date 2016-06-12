class CheckBox;

#ifndef DEF_CHECK_BOX
#define DEF_CHECK_BOX

#include "../c_Window.h"

class CheckBox : public Window{
public:
	CheckBox(const wchar* caption,int x,int y,int w,int h,Window *parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~CheckBox();
	void setCheck(int c);
	int getCheck();
};





#endif
