#pragma once

#include "common/_required.h" //u16

static const double PI=3.14159265;

namespace math{
	class Aabb3;
	class Line3;
	class Obb3;
	class Plane3;
	class Segment3;

	template <int ROWS,int COLS> class Mtx;
	typedef Mtx<4,4> Mtx44;
	typedef Mtx<3,4> Mtx34;
	typedef Mtx<3,3> Mtx33;

	template <class T> class RectBase;
	typedef RectBase<long>			CRect;
	typedef RectBase<double>		CRectF;

	template <class T> class BoxSizeBase;
	typedef BoxSizeBase<long>		CBoxSize;
	typedef BoxSizeBase<double>		CBoxSizeF;

	template <class T> class Vec2Base;
	typedef Vec2Base<long>			CVec2;
	typedef Vec2Base<double>		CVec2F;
	typedef CVec2					CPoint;

	template <class T,int UNIT> class AngleBase;
	typedef AngleBase<double,1>		Rad;
	typedef AngleBase<u16,65536>	Idx;
	typedef Rad						Angle;

	template <class T> class BoxSizeBase;
	typedef BoxSizeBase<long>		CBoxSize;
	typedef BoxSizeBase<double>		CBoxSizeF;
}

using namespace math;




