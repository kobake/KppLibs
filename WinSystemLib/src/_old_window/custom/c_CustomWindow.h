class CustomWindow;

#ifndef _CustomWindow_
#define _CustomWindow_

#include "c_Window.h"

#define CUSTOMWINDOWF_KIND(option) (option & 0x0007)

#define CUSTOMWINDOWF_KIND_NORMAL	0x0000
#define CUSTOMWINDOWF_KIND_TOOL		0x0001
#define CUSTOMWINDOWF_KIND_POPUP	0x0002
#define CUSTOMWINDOWF_KIND_DIALOG	0x0003
#define CUSTOMWINDOWF_KIND_CANVAS	0x0004

class CustomWindow : public Window{
public:
	void _init_v();
	CustomWindow(const wstring& caption,int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~CustomWindow();
};

#endif
