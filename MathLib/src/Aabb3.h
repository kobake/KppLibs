#pragma once

#include "Vec3.h"
#include "Mtx.h"

namespace math{

//�����s���E�{�b�N�X AABB (axis-aligned bounding box)
class Aabb3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//����
	static void createByMinMax(Aabb3* aabb,const t_Vec3& min_pos,const t_Vec3& max_pos);
	static void createByCenterAndLengths(Aabb3* aabb,const t_Vec3& center,const t_Vec3& half_lengths);
private:
	void _check() const;
public:
	//�R���X�g���N�^
//	Aabb3(const t_Vec3& center,const t_Vec3& half_lengths);
	Aabb3();
	Aabb3(const Aabb3& rhs);
	void Assign(const Aabb3& rhs);
	Aabb3& operator = (const Aabb3& rhs){ Assign(rhs); return *this; }
	//�擾
	const t_Vec3& getCenter() const		{ return c; }
	const t_Vec3& getHalfLengths() const{ return r; }
	t_Vec3 getMin() const{ return c-r; }
	t_Vec3 getMax() const{ return c+r; }
	//�ݒ�
	void setCenterAndLengths(const t_Vec3& center,const t_Vec3& half_lengths);
	void setMinMax(const t_Vec3& min_pos,const t_Vec3& max_pos);
	//����
	void merge(const Aabb3& aabb);
	//�ϊ�
	void transform(const Mtx<3,4>& mtx);
	//�c�[��
	bool intersects(const Aabb3& obb) const; //��������
	void applyTranslate(const t_Vec3& v);	 //���s�ړ�(�t)
	void applyRotate(const t_Vec3& v);		 //��]    (�s�t)
	void applyScale(const t_Vec3& v);		 //�X�P�[��(�t)
private:
	t_Vec3 c;
	t_Vec3 r;
};


} //namespace math
