#include <windows.h>
#include "common/_required.h"
#include "c_RectCursor.h"
#include "util/std.h"

RectCursor::RectCursor()
{
	setXYWH(0,0,1,1);
}
//•Ï‰»‚ª‚ ‚Á‚½‚ç0ˆÈŠO‚ð•Ô‚·
int RectCursor::setXYXY(int x1,int y1,int x2,int y2)
{
	int _x=tmin<int>(x1,x2);
	int _y=tmin<int>(y1,y2);
	int _w=abs(x1-x2)+1;
	int _h=abs(y1-y2)+1;
	return setXYWH(_x,_y,_w,_h);
}
//•Ï‰»‚ª‚ ‚Á‚½‚ç0ˆÈŠO‚ð•Ô‚·
int RectCursor::setXYWH(int _x,int _y,int _w,int _h)
{
	int ret=abs(x()-_x)+abs(y()-_y)+abs(w()-_w)+abs(h()-_h);
	rc.left=_x;
	rc.top=_y;
	rc.right=_x+_w;
	rc.bottom=_y+_h;
	return ret;
}
bool RectCursor::eqXYXY(int x1,int y1,int x2,int y2)
{
	int _x=tmin<int>(x1,x2);
	int _y=tmin<int>(y1,y2);
	int _w=abs(x1-x2)+1;
	int _h=abs(y1-y2)+1;
	return eqXYWH(_x,_y,_w,_h);
}
bool RectCursor::eqXYWH(int _x,int _y,int _w,int _h)
{
	return (x()==_x && y()==_y && w()==_w && h()==_h);
}
