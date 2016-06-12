#pragma once

#include "../c_Window.h"

class ComboBox : public Window{
protected:
	int drop_h;
public:
	ComboBox(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~ComboBox();
	int addString(const wchar *str);
	int getSelected(){ return (int)SendMessage(hwnd,CB_GETCURSEL,0,0); }
	void setSelected(int s){ SendMessage(hwnd,CB_SETCURSEL,s,0); }
	virtual void move(int x,int y,int w,int h)
	{
		Window::move(x,y,w,drop_h);
	}
};

