#include "common/_required.h"
#include <windows.h>
#include "c_MapScreen.h"
#include "f_base.h"
//#include "win/Cast.h"
#include <WinSystemLib.h>

MapScreen::MapScreen()
{
	map=NULL;
	map_x=0;
	map_y=0;
}
void MapScreen::setMap(VirtualMap *_map)
{
	map=_map;
}
void MapScreen::setMapX(int _map_x)
{
	map_x=_map_x;
}
void MapScreen::setMapY(int _map_y)
{
	map_y=_map_y;
}
void MapScreen::setMapXY(int _map_x,int _map_y)
{
	map_x=_map_x;
	map_y=_map_y;
}
void MapScreen::offsetMapXY(int dx,int dy)
{
	map_x+=dx;
	map_y+=dy;
}
#if 0
void MapScreen::drawCursor(CDcGraphics *g,RECT *rcUpdate,RectCursor *cursor,const CColor& color)
{
	RECT rc=cursor->getRect();
	//
	OffsetRect(&rc,-map_x,-map_y);
	//
	rc.left*=map->getPixelsPerX();
	rc.top*=map->getPixelsPerY();
	rc.right*=map->getPixelsPerX();
	rc.bottom*=map->getPixelsPerY();
	//
	RECT tmp;
	if(IntersectRect(&tmp,rcUpdate,&rc)){
		if(color.a==1){
			g->getPen()->SetColor(CColor(0,0,0));
			g->setInvert(true);
		}else{
			g->getPen()->SetColor(color);
		}
		g->getPen()->setWidth(2);
		g->getPen()->setStyle(Pen::Inside);

		g->drawRect(RECT2Rect(rc));
		g->setInvert(false);
	}
}
void MapScreen::drawMap(CDcGraphics *g,RECT *rcUpdate)
{
	::drawMap(g,&Rect2RECT(g->getInvalidRect()),map,map_x,map_y,CColor(0,0,0));
}
#endif
//ƒ}ƒbƒv“à•”‚Å‚ ‚ê‚Î true
bool MapScreen::screenToMap(POINT *pt)
{
	POINT ret=*pt;
	ret.x+=map_x*map->getPixelsPerX();
	ret.y+=map_y*map->getPixelsPerY();
	ret.x/=map->getPixelsPerX();
	ret.y/=map->getPixelsPerY();
	*pt=ret;
	return map->ptIn(pt);
}
RECT MapScreen::getCursorRect(RectCursor *cursor)
{
	RECT rc=cursor->getRect();
	//
	OffsetRect(&rc,-map_x,-map_y);
	//
	rc.left*=map->getPixelsPerX();
	rc.top*=map->getPixelsPerY();
	rc.right*=map->getPixelsPerX();
	rc.bottom*=map->getPixelsPerY();
	//
	return rc;
}

POINT MapScreen::getScrollRange(const RECT *rcScreen)
{
	POINT ret;
	int n;
	n=map->getDataWidth()-(rcScreen->right-rcScreen->left)/map->getPixelsPerX(); if(n<0)n=0;
	ret.x=n;
	n=map->getDataHeight()-(rcScreen->bottom-rcScreen->top)/map->getPixelsPerY(); if(n<0)n=0;
	ret.y=n;
	return ret;
}

void MapScreen::adjustMapXY(int x_max,int y_max)
{
	setMapX(adjust(getMapX(),0,x_max));
	setMapY(adjust(getMapY(),0,y_max));
}
