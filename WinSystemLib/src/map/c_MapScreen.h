#pragma once

#include <GraphicsLib.h> // CColor

class VirtualMap;
class RectCursor;
class ScrollManager;

class MapScreen{
protected:
	VirtualMap *map;
	int map_x;
	int map_y;
public:
	MapScreen();
	void setMap(VirtualMap *_map);
	void setMapX(int _map_x);
	void setMapY(int _map_y);
	void setMapXY(int _map_x,int _map_y);
	int getMapX(){ return map_x; }
	int getMapY(){ return map_y; }
	void offsetMapXY(int dx,int dy);
	void drawMap(CDcGraphics *g,RECT *rcUpdate);
	bool screenToMap(POINT *pt);
	RECT getCursorRect(RectCursor *cursor);
	POINT getScrollRange(const RECT *rcScreen);
	void adjustMapXY(int x_max,int y_max);
	void drawCursor(CDcGraphics *g,RECT *rcUpdate,RectCursor *cursor,const CColor& color=CColor(255,0,0));

/*	void setScrollRange(const RECT *rcScreen)
	{
		POINT pt=getScrollRange(rcScreen);
		hscroll->setRange(0,pt.x);
		canvas.setMapX(adjust(canvas.getMapX(),0,pt.x));
		vscroll->setRange(0,pt.y);
		canvas.setMapY(adjust(canvas.getMapY(),0,pt.y));
	}
	void onScroll(UINT msg,WPARAM wParam,LPARAM lParam)
	{
		int x=hscroll->getPos();
		int y=vscroll->getPos();
		hscroll->onScroll(msg,wParam,lParam);
		vscroll->onScroll(msg,wParam,lParam);
		int dx=hscroll->getPos()-x;
		int dy=vscroll->getPos()-y;
		if(dx!=0 || dy!=0){
			canvas.offsetMapXY(dx,dy);
			ScrollWindowEx(hwnd,-dx*3,-dy*3,NULL,this->getClientRect(),NULL,NULL,SW_INVALIDATE);
		}
	}*/
};

#include "c_VirtualMap.h"
#include "c_RectCursor.h"


