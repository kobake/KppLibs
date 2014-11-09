#pragma once

#include "sdk/libfwd_win.h"
#include "../../MathLib/MathLib.h"

math::CRect RECT2Rect(const RECT& rc);
RECT Rect2RECT(const math::CRect& rc);

math::CPoint POINT2Point(const POINT& pt);
POINT Point2POINT(const math::CPoint& pt);

math::CBoxSize SIZE2Size(const SIZE& size);
SIZE Size2SIZE(const math::CBoxSize& size);

void* LongToPointerImp(LONG n);
LONG PointerToLongImp(void* p);
DWORD PointerToDwordImp(void* p);

template <class T>
inline T	LongToPointer(LONG  n){ return reinterpret_cast<T>(LongToPointerImp(n)); }
inline LONG	PointerToLong(void* p){ return PointerToLongImp(p); }
inline DWORD PointerToDword(void* p){ return PointerToDwordImp(p); }
