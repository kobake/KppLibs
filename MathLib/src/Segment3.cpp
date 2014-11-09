#include "_required.h"
#include "Segment3.h"

namespace math{

//初期化無し
Segment3::Segment3()
{
}

//任意の2点により定義
Segment3::Segment3(const t_Vec3& p0,const t_Vec3& p1)
{
	pos[0]=p0;
	pos[1]=p1;
}

//任意の2点(配列)により定義
Segment3::Segment3(const t_Vec3* p)
{
	pos[0]=p[0];
	pos[1]=p[1];
}


} //namespace math
