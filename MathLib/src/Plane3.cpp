#include "_required.h"
#include "Plane3.h"

namespace math{


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         Plane3                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

/*
//ax+by+cz+d=0�̕��ʂ��`
Plane3::Plane3(f32 a,f32 b,f32 c,f32 d)
: normal(a,b,c)
{
	p0=?;
}
*/

//����������
Plane3::Plane3()
{
}

//�C�ӂ̓_�Ɩ@���x�N�g���ɂ���`
Plane3::Plane3(const t_Vec3& p,const t_Vec3& v)
: pos0(p), normal(v)
{
}

//�C�ӂ�3�_�ɂ���`
Plane3::Plane3(const t_Vec3& p0,const t_Vec3& p1,const t_Vec3 p2)
{
	pos0=p0;
	cross_product(&normal,p1-p0,p2-p0);
}

//�C�ӂ�3�_(�z��)�ɂ���`
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
