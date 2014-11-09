#pragma once

#include "Vec3.h"

namespace math{

//�������`
class Line3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//����
	static void constructByPosVector(Line3* line,const t_Vec3& p0,const t_Vec3& v); //�C�ӂ̓_�ƕ����x�N�g���ɂ���`
	static void constructByPosPos(Line3* line,const t_Vec3& p0,const t_Vec3& p1);   //�C�ӂ�2�_�ɂ���`
	static void constructByPosPos(Line3* line,const t_Vec3* p);                     //�C�ӂ�2�_(�z��)�ɂ���`
private:
	void _check() const;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Line3();                                  //����������
	Line3(const t_Vec3& p0,const t_Vec3& v);  //�C�ӂ̓_�ƕ����x�N�g���ɂ���`
	Line3(const Line3& rhs);
	Line3& operator = (const Line3& rhs);
	//�擾
	const t_Vec3& getPosSample() const{ return pos0; }
	const t_Vec3& getDirection() const{ return dir; }
private:
	t_Vec3 pos0;
	t_Vec3 dir;
};



} //namespace math
