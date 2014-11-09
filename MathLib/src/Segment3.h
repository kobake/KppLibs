#pragma once

#include "Vec3.h"

namespace math{

//�������`
//segment line
class Segment3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Segment3();                                  //����������
	Segment3(const t_Vec3& p0,const t_Vec3& p1); //�C�ӂ�2�_�ɂ���`
	Segment3(const t_Vec3* p);                   //�C�ӂ�2�_(�z��)�ɂ���`
	//�C���^�[�t�F�[�X
	const t_Vec3& getBegin() const{ return pos[0]; }
	const t_Vec3& getEnd() const{ return pos[1]; }
private:
	t_Vec3 pos[2];
};




} //namespace math
