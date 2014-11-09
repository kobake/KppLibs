#pragma once

namespace math{

//#include "Mtx_fwd.h"
template <int ROWS,int COLS> class Mtx;
typedef Mtx<3,4> Mtx34;
typedef Mtx<4,4> Mtx44;

template <class T>
class Vec3Base{
public:
	T x;
	T y;
	T z;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Vec3Base() : x(0), y(0), z(0) { }
	Vec3Base(T _x,T _y,T _z) : x(_x), y(_y), z(_z) { }
	template <class T2>
		Vec3Base(const Vec3Base<T2>& rhs) : x((T)rhs.x), y((T)rhs.y), z((T)rhs.z) { }
	//���
	Vec3Base& operator = (const Vec3Base& rhs){ Assign(rhs); return *this; }
	void Assign(T _x,T _y,T _z){ x=_x; y=_y; z=_z; }
	void Assign(const Vec3Base& rhs){ x=rhs.x; y=rhs.y; z=rhs.z; }
	//�A�N�Z�X�⏕ (�z��Ƃ��Ă��A�N�Z�X�ł���悤��)
	T& at(int i)						{ return (&x)[i]; }
	const T& at(int i) const			{ return (&x)[i]; }
	T& operator[](int i)				{ return (&x)[i]; }
	const T& operator[](int i) const	{ return (&x)[i]; }
	//���Z�q���b�v
	Vec3Base& operator += (const Vec3Base& v){ x+=v.x; y+=v.y; z+=v.z; return *this; } 
	Vec3Base& operator -= (const Vec3Base& v){ x-=v.x; y-=v.y; z-=v.z; return *this; } 
	Vec3Base& operator *= (T p){ x*=p; y*=p; z*=p; return *this; }
	Vec3Base& operator /= (T p){ x/=p; y/=p; z/=p; return *this; }
	Vec3Base operator + (const Vec3Base& v) const{ Vec3Base ret=*this; return ret+=v; }
	Vec3Base operator - (const Vec3Base& v) const{ Vec3Base ret=*this; return ret-=v; }
	Vec3Base operator * (T p) const{ Vec3Base ret=*this; return ret*=p; }
	Vec3Base operator / (T p) const{ Vec3Base ret=*this; return ret/=p; }
	Vec3Base operator + () const{ return *this; }
	Vec3Base operator - () const{ return Vec3Base(-x,-y,-z); }
	//��Βl
	T length() const;
	//���K��
	Vec3Base& normalize(){ (*this)/=length(); return *this; }
	Vec3Base normalized() const{ Vec3Base ret=*this; ret.normalize(); return ret; }
	//�s��ɂ��ϊ�
	void transform(const Mtx34& mtx);
	void transform(const Mtx44& mtx);
	void transformVector(const Mtx34& mtx);
	void transformVector(const Mtx44& mtx);
	//���p�x�̕ϊ�
	Vec3Base& applyTranslate(const Vec3Base& v);	//���s�ړ�
	Vec3Base& applyRotate(const Vec3Base& v);		//��]
	Vec3Base& applyScale(const Vec3Base& v);		//�X�P�[��
	//���[�e�B���e�B
	T getRoll() const;
	T getYaw() const;
	T getPitch() const;
};


//���� ( inner product / dot product )
template <class T>
T dot_product(const Vec3Base<T>& v0,const Vec3Base<T>& v1);

//�O�� ( outer product / cross product )
template <class T>
Vec3Base<T> cross_product(const Vec3Base<T>& v0,const Vec3Base<T>& v1);

template <class T>
void cross_product(Vec3Base<T>* in,const Vec3Base<T>& v0,const Vec3Base<T>& v1);


} //namespace math
