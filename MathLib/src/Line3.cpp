#include "_required.h"
#include "Line3.h"
#include <DebugLib.h>

namespace math{


	
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           生成                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//任意の点と方向ベクトルにより定義
void Line3::constructByPosVector(Line3* line,const t_Vec3& p0,const t_Vec3& v)
{
	line->pos0=p0;
	line->dir=v;
	line->dir.normalize();
	line->_check();
}

//任意の2点により定義
void Line3::constructByPosPos(Line3* line,const t_Vec3& p0,const t_Vec3& p1)
{
	line->pos0=p0;
	line->dir=p1-p0;
	line->dir.normalize();
	line->_check();
}

//任意の2点(配列)により定義
void Line3::constructByPosPos(Line3* line,const t_Vec3* p)
{
	line->pos0=p[0];
	line->dir=p[1]-p[0];
	line->dir.normalize();
	line->_check();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Line3::_check() const
{
#ifdef _DEBUG
	if(dir.length()==0){
		debugPrint(L"---------- !!WARNING!! ----------\n");
		debugPrint(L"----- Line3 : zero direction vector -----\n");
	}
#endif
}

//初期化無し
Line3::Line3()
{
}

//任意の点とベクトルにより定義
Line3::Line3(const t_Vec3& p0,const t_Vec3& v)
: pos0(p0), dir(v)
{
	dir.normalize();
}

//コピー
Line3::Line3(const Line3& rhs)
: pos0(rhs.pos0), dir(rhs.dir)
{
}

Line3& Line3::operator = (const Line3& rhs)
{
	pos0=rhs.pos0;
	dir=rhs.dir;
	return *this;
}



} //namespace math
