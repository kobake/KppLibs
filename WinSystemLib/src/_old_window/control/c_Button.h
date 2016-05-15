class Button;

#ifndef DEF_BUTTON
#define DEF_BUTTON

#include "c_Window.h"

#define BUTTONF_KIND(option) (option &	0x0003)
#define BUTTONF_KIND_NORMALBUTTON		0x0000
#define BUTTONF_KIND_DEFAULTBUTTON		0x0001

class Button : public Window{
public:
	Button(const wchar* caption,int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~Button();
};





#endif
