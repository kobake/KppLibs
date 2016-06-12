class RectCursor;

#ifndef DEF_RECT_CURSOR
#define DEF_RECT_CURSOR

class RectCursor{
protected:
	int rc_x,rc_y;
	int rc_w,rc_h;
	RECT rc;
public:
	RectCursor();
	int x(){ return rc.left;          }
	int y(){ return rc.top;           }
	int w(){ return rc.right-rc.left; }
	int h(){ return rc.bottom-rc.top; }
	int l(){ return rc.left;          }
	int t(){ return rc.top;           }
	int r(){ return rc.right;         }
	int b(){ return rc.bottom;        }
	RECT getRect(){ return rc; }
	int size(){ return w()*h(); }
	//
	int setXYXY(int x1,int y1,int x2,int y2);
	int setXYWH(int _x,int _y,int _w,int _h);
	bool eqXYXY(int x1,int y1,int x2,int y2);
	bool eqXYWH(int _x,int _y,int _w,int _h);
};

#endif
