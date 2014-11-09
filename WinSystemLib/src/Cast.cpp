#include "_required.h"
#include "Cast.h"
#include "../../MathLib/MathLib.h" //CRect
#include <DebugLib.h>



math::CRect RECT2Rect(const RECT& rc)
{
	return math::CRect(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top);
}

RECT Rect2RECT(const math::CRect& rc)
{
	RECT out={rc.Left(),rc.Top(),rc.Right(),rc.Bottom()};
	return out;
}

math::CPoint POINT2Point(const POINT& pt)
{
	return math::CPoint(pt.x,pt.y);
}

POINT Point2POINT(const math::CPoint& pt)
{
	POINT out={pt.x,pt.y};
	return out;
}

math::CBoxSize SIZE2Size(const SIZE& size)
{
	return math::CBoxSize(size.cx,size.cy);
}

SIZE Size2SIZE(const math::CBoxSize& size)
{
	SIZE out={size.w,size.h};
	return out;
}


void* LongToPointerImp(LONG n)
{
	return (void*)(intptr_t)n;
}

LONG PointerToLongImp(void* p)
{
	return (LONG)(intptr_t)p;
}

DWORD PointerToDwordImp(void* p)
{
	return (DWORD)(intptr_t)p;
}
