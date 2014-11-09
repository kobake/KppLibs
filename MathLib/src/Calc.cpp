#include "_required.h"
#include "Calc.h"
#include "Vec3.h"
#include "Plane3.h"
#include "Line3.h"

namespace math{

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �v�Z                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//�ʂƒ����̌�_
bool calc_intersection(Vec3Base<double>* out,const Plane3& plane,const Line3& line)
{
	double t2=dot_product(plane.getNormal(),line.getDirection()); //����
	if(t2==0)return false; //�����Ɩʂ����s�A�܂���line�̕����x�N�g����0
	double t1=dot_product(plane.getNormal(),plane.getPosSample()-line.getPosSample()); //���q
	double t=t1/t2;
	*out=line.getPosSample()+line.getDirection()*t;
	return true;
}




} //namespace math
