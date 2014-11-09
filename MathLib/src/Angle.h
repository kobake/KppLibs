#pragma once

#include "_fwd.h"

namespace math{

//!�p�x�N���X�e���v���[�g�B���W�n�� X���� Y����
/*!
@param T �����l�̌^
@param U ���]�̒l�B���������ʂɁAU=1 �� U=PI*2 �Ƃ݂Ȃ�
*/
template <class T,int U>
class AngleBase{
private:
	typedef AngleBase<T,U> Me;
public:
	//!1��]�̒l���擾
	static double UNIT()
	{
		if(U==1)return PI*2;
		else return U;
	}
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	AngleBase(T _v=0) : v(_v) { }
	template <class T2,int U2> explicit AngleBase(const AngleBase<T2,U2>& rhs);
	//�Z�p���Z�q
	AngleBase& operator += (const Me& rhs){ v=(T)(v+rhs.v); return *this; }
	AngleBase& operator -= (const Me& rhs){ v=(T)(v-rhs.v); return *this; }
	AngleBase& operator *= (double n){ v=(T)(v*n); return *this; }
	AngleBase& operator /= (double n){ v=(T)(v/n); return *this; }
	//��r���Z�q
	bool operator == (const Me& n){ return v==n; }
	bool operator != (const Me& n){ return v!=n; }
	//�l���o��
	T value() const{ return v; }
	operator T() const{ return v; }
	//�x�N�g���ϊ�
	math::CVec2F toVec() const;
private:
	T v;
};

//1��]=2��[rad]
typedef AngleBase<double,1> Rad;

//1��]=65536[idx]
typedef AngleBase<u16,65536> Idx;

//�W���^
typedef Rad Angle;

} //namespace math

//using namespace math;

