#pragma once

#include "CVec2.h"

namespace math{

/*
class Polygon2{
public:
	//�����ɒ�`
	//3�ȏ�̔C�ӓ_�ɂ���`
	Polygon2(const t_Vec2* points,int npoints);
	//�擾
	size_t size();
	t_Vec2 at(int index);
	//�c�[��
	CRect getBounds() const; //�o�E���f�B���O�{�b�N�X���擾
private:

};
*/

class Quad2{
public:
	typedef double ValueType;
	typedef Vec2Base<ValueType> t_Vec2;
	typedef RectBase<ValueType> t_Rect;
public:
	//�����ɒ�`
	Quad2(const t_Vec2& p0,const t_Vec2& p1,const t_Vec2& p2,const t_Vec2& p3); //�C�ӂ�4�_�ɂ���`
	Quad2(const t_Vec2* p);                                                     //�C�ӂ�4�_(�z��)�ɂ���`
	Quad2(const Quad2& rhs);
	Quad2();
	void Assign(const Quad2& rhs);
	Quad2& operator = (const Quad2& rhs){ Assign(rhs); return *this; }
	//�擾
	size_t size() const{ return 4; }
	const t_Vec2& at(int i) const{ return pos[i]; }
	t_Vec2& at(int i){ return pos[i]; }
	//���Z�q
	Quad2& operator += (const t_Vec2& v);
	Quad2& operator -= (const t_Vec2& v);
	//���Z�q���b�v
	Quad2 operator + (const t_Vec2& v) const{ Quad2 ret=*this; return ret+=v; }
	Quad2 operator - (const t_Vec2& v) const{ Quad2 ret=*this; return ret-=v; }
	//�c�[��
	bool contains(const t_Vec2& v) const; //�����
	bool intersects(const Quad2& quad) const; //��������
	t_Rect getBounds() const; //�o�E���f�B���O�{�b�N�X���擾
private:
	t_Vec2 pos[4];
};


} //namespace math
