#include "_required.h"
#include "Segment3.h"

namespace math{

//����������
Segment3::Segment3()
{
}

//�C�ӂ�2�_�ɂ���`
Segment3::Segment3(const t_Vec3& p0,const t_Vec3& p1)
{
	pos[0]=p0;
	pos[1]=p1;
}

//�C�ӂ�2�_(�z��)�ɂ���`
Segment3::Segment3(const t_Vec3* p)
{
	pos[0]=p[0];
	pos[1]=p[1];
}


} //namespace math
