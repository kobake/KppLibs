#include "_required.h"
#include "Angle.h"
#include <math.h>
#include "CVec2.h"

namespace math{

template <class T,int U>
template <class T2,int U2>
AngleBase<T,U>::AngleBase(const AngleBase<T2,U2>& rhs)
{
	v=rhs.v*UNIT()/rhs.UNIT();
}

template <class T,int U>
CVec2F AngleBase<T,U>::toVec() const
{
	double v2=v*PI*2/UNIT();
	return math::CVec2F(cos(v2),sin(v2));
}


//明示的なインスタンス可
template class AngleBase<double,1>; //Rad
template class AngleBase<u16,65536>; //Idx


} //namespace math
