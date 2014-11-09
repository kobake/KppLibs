#include "_required.h"
#include "Line3.h"
#include <DebugLib.h>

namespace math{


	
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//�C�ӂ̓_�ƕ����x�N�g���ɂ���`
void Line3::constructByPosVector(Line3* line,const t_Vec3& p0,const t_Vec3& v)
{
	line->pos0=p0;
	line->dir=v;
	line->dir.normalize();
	line->_check();
}

//�C�ӂ�2�_�ɂ���`
void Line3::constructByPosPos(Line3* line,const t_Vec3& p0,const t_Vec3& p1)
{
	line->pos0=p0;
	line->dir=p1-p0;
	line->dir.normalize();
	line->_check();
}

//�C�ӂ�2�_(�z��)�ɂ���`
void Line3::constructByPosPos(Line3* line,const t_Vec3* p)
{
	line->pos0=p[0];
	line->dir=p[1]-p[0];
	line->dir.normalize();
	line->_check();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
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

//����������
Line3::Line3()
{
}

//�C�ӂ̓_�ƃx�N�g���ɂ���`
Line3::Line3(const t_Vec3& p0,const t_Vec3& v)
: pos0(p0), dir(v)
{
	dir.normalize();
}

//�R�s�[
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
