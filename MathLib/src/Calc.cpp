#include "_required.h"
#include "Calc.h"
#include "Vec3.h"
#include "Plane3.h"
#include "Line3.h"

namespace math{

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           計算                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//面と直線の交点
bool calc_intersection(Vec3Base<double>* out,const Plane3& plane,const Line3& line)
{
	double t2=dot_product(plane.getNormal(),line.getDirection()); //分母
	if(t2==0)return false; //直線と面が平行、またはlineの方向ベクトルが0
	double t1=dot_product(plane.getNormal(),plane.getPosSample()-line.getPosSample()); //分子
	double t=t1/t2;
	*out=line.getPosSample()+line.getDirection()*t;
	return true;
}




} //namespace math
