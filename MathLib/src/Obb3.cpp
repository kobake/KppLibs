#include "_required.h"
#include "Obb3.h"
#include "Aabb3.h"
#include <BaseLib.h>
#include "CRect.h"
#include "Mtx.h"


namespace math{

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Obb3::createByAabb3(Obb3* obb,const Aabb3& aabb)
{
	obb->c=aabb.getCenter();
	obb->u[0].Assign(1,0,0);
	obb->u[1].Assign(0,1,0);
	obb->u[2].Assign(0,0,1);
	obb->e=aabb.getHalfLengths();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Obb3::Obb3(const t_Vec3& center,
	const t_Vec3& x_axis,const t_Vec3& y_axis,const t_Vec3& z_axis,
	const t_Vec3& lengths)
{
	c=center;
	u[0]=x_axis;
	u[1]=y_axis;
	u[2]=z_axis;
	e=lengths;
}

Obb3::Obb3()
{
	//�����(0,0)
}

Obb3::Obb3(const Obb3& rhs)
{
	Assign(rhs);
}

void Obb3::Assign(const Obb3& rhs)
{
	c=rhs.c;
	u[0]=rhs.u[0];
	u[1]=rhs.u[1];
	u[2]=rhs.u[2];
	e=rhs.e;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �ϊ�                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Obb3::transform(const Mtx<3,4>& mtx)
{
	//���S
	c.transform(mtx);
	//��
	u[0].transformVector(mtx);
	u[1].transformVector(mtx);
	u[2].transformVector(mtx);
	//����
//	e.transformVector(mtx);
	//���̐��K���ƒ������킹
	for(int i=0;i<3;i++){
		e[i]*=u[i].length();
		u[i].normalize();
	}
}



} //namespace math
