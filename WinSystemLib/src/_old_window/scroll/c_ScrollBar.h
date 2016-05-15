class ScrollBar;

#ifndef _ScrollBar_
#define _ScrollBar_

#include "c_Window.h"
#include "c_Graphics.h"


#define SCROLLBARF_KIND(option) (option & 0x0001)
#define SCROLLBARF_KIND_HORZ	0x0000
#define SCROLLBARF_KIND_VERT	0x0001

class ScrollBar : public Window{
public:
	//コンストラクタ・デストラクタ
	void _init_v();
	ScrollBar(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	~ScrollBar();
	//
};

#endif
