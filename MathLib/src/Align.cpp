#include "_required.h"
#include "Align.h"
#include "CVec2.h"
#include "BoxSize.h"

namespace math{

template <class T>
T Align::adjustX(T x,T w) const
{
	if(align & X_CENTER){
		x-=w/2;
	}else if(align & X_RIGHT){
		x-=w;
	}
	return x;
}

template <class T>
T Align::adjustY(T y,T h) const
{
	if(align & Y_CENTER){
		y-=h/2;
	}else if(align & Y_BOTTOM){
		y-=h;
	}
	return y;
}

template <class T>
Vec2Base<T> Align::adjust(const Vec2Base<T>& pos,const BoxSizeBase<T>& size) const
{
	return Vec2Base<T>(adjustX(pos.x,size.w),adjustY(pos.y,size.h));
}


// -- -- 明示的なインスタンス化 -- -- //
template double Align::adjustX(double,double) const;
template double Align::adjustY(double,double) const;
template Vec2Base<double> Align::adjust(const Vec2Base<double>& pos,const math::CBoxSizeF& size) const;

template long Align::adjustX(long,long) const;
template long Align::adjustY(long,long) const;
template Vec2Base<long> Align::adjust(const Vec2Base<long>& pos,const math::CBoxSize& size) const;


} //namespace math
