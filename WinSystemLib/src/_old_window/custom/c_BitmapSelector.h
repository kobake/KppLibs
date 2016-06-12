#pragma once

#include "../scroll/c_ScrollManager.h"
#include "../../map/c_RectCursor.h"
#include "../c_Window.h"
#include <BaseLib.h> // t_Table2D
#include "../scroll/c_ScrollDataWindow.h"
#include <GraphicsLib.h> //CDib

class BitmapSelector : public ScrollDataWindow{
protected:
	bool isValidData(){ return bmp!=NULL; }
	int getDataNW(){ return bmp->GetSize().w/32; }
	int getDataNH(){ return bmp->GetSize().h/32; }
	void drawData(CDcGraphics *g,int xoff,int yoff);
protected:
	CDib* bmp;
public:
	BitmapSelector(const wstring& caption,int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~BitmapSelector();
	void setBitmap(CDib *_bmp);
	void realizeBitmap();
};


