#include "_required.h"
#include "Plane3.h"

namespace math{


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         Plane3                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

/*
//ax+by+cz+d=0の平面を定義
Plane3::Plane3(f32 a,f32 b,f32 c,f32 d)
: normal(a,b,c)
{
	p0=?;
}
*/

//初期化無し
Plane3::Plane3()
{
}

//任意の点と法線ベクトルにより定義
Plane3::Plane3(const t_Vec3& p,const t_Vec3& v)
: pos0(p), normal(v)
{
}

//任意の3点により定義
Plane3::Plane3(const t_Vec3& p0,const t_Vec3& p1,const t_Vec3 p2)
{
	pos0=p0;
	cross_product(&normal,p1-p0,p2-p0);
}

//任意の3点(配列)により定義
Plane3::Plane3(t_Vec3* p)
{
	pos0=p[0];
	cross_product(&normal,p[1]-p[0],p[2]-p[0]);
}


Plane3::Plane3(const Plane3& rhs)
: pos0(rhs.pos0), normal(rhs.normal)
{
}

} //namespace math
