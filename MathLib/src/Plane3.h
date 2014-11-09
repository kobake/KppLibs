#pragma once

#include "Vec3.h"

namespace math{

//���ʂ��`
class Plane3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	//Plane3(f32 a,f32 b,f32 c,f32 d);                         //ax+by+cz+d=0�̕��ʂ��`(������)
	Plane3();                                                  //����������
	Plane3(const t_Vec3& p,const t_Vec3& v);                   //�C�ӂ̓_�Ɩ@���x�N�g���ɂ���`
	Plane3(const t_Vec3& p0,const t_Vec3& p1,const t_Vec3 p2); //�C�ӂ�3�_�ɂ���`
	Plane3(t_Vec3* p);                                         //�C�ӂ�3�_(�z��)�ɂ���`
	Plane3(const Plane3& rhs);
	Plane3& operator = (const Plane3& rhs){ pos0=rhs.pos0; normal=rhs.normal; return *this; }
	//�擾
	const t_Vec3& getPosSample() const{ return pos0; }
	const t_Vec3& getNormal() const{ return normal; }
private:
	t_Vec3 pos0;
	t_Vec3 normal;
};


} //namespace math
