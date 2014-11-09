#pragma once

#include "Vec3.h"

namespace math{

class Aabb3;
class Intersect3;

//�L�����E�{�b�N�X (OBB)
class Obb3{
public:
	friend class Intersect3;
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//����
	static void createByAabb3(Obb3* obb,const Aabb3& aabb);
public:
	//�R���X�g���N�^
	Obb3(const t_Vec3& center,
		const t_Vec3& x_axis,const t_Vec3& y_axis,const t_Vec3& z_axis,
		const t_Vec3& lengths);
	Obb3();
	Obb3(const Obb3& rhs);
	void Assign(const Obb3& rhs);
	Obb3& operator = (const Obb3& rhs){ Assign(rhs); return *this; }
	//�擾
	const t_Vec3& center()		const{ return c; }
	const t_Vec3& axis(int i)	const{ return u[i]; }
	const t_Vec3& lengths()		const{ return e; }
	t_Vec3& center()	{ return c; }
	t_Vec3& axis(int i)	{ return u[i]; }
	t_Vec3& lengths()	{ return e; }
	//�ϊ�
	void transform(const Mtx<3,4>& mtx);
private:
	t_Vec3 c;    //OBB�̒��S�_                          box center
	t_Vec3 u[3]; //���[�J��x,y,z��                      local x,y,z axis
	t_Vec3 e;    //OBB�̊e���ɉ����Đ��̕��̔����͈̔�  positive half-edge lengths
};


} //namespace math
