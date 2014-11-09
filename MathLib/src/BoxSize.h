#pragma once

namespace math{

template <class T> class RectBase;

template <class T>
class BoxSizeBase{
private:
	typedef RectBase<T> t_Rect;
public:
	//��WindowsSDK �� SIZE �Ɠ����\���ɂ��Ă��� (�O�̂���)
	T w;
	T h;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	explicit BoxSizeBase(const t_Rect& rc);
	BoxSizeBase(T _w=0,T _h=0) : w(_w), h(_h) { }
	//���
	void Assign(T _w,T _h){ w=_w; h=_h; }
	void set(T _w,T _h){ w=_w; h=_h; }
	//��r
	bool eq(const BoxSizeBase& right) const{ return w==right.w && h==right.h; }
	//��r���Z�q
	bool operator == (const BoxSizeBase& right) const{ return eq(right); }
	bool operator != (const BoxSizeBase& right) const{ return !eq(right); }
	//���Z�q
	BoxSizeBase& operator *= (T n){ w*=n; h*=n; return *this; }
	BoxSizeBase& operator /= (T n){ w/=n; h/=n; return *this; }
	//���Z�q���b�p�[
	const BoxSizeBase operator * (T n) const{ BoxSizeBase ret=*this; return ret*=n; }
	const BoxSizeBase operator / (T n) const{ BoxSizeBase ret=*this; return ret/=n; }
	//���Z�q
	BoxSizeBase& operator += (const BoxSizeBase& size){ w+=size.w; h+=size.h; return *this; }
	BoxSizeBase& operator -= (const BoxSizeBase& size){ w-=size.w; h-=size.h; return *this; }
	//���Z�q���b�p�[
	const BoxSizeBase operator + (const BoxSizeBase& size) const{ BoxSizeBase ret=*this; return ret+=size; }
	const BoxSizeBase operator - (const BoxSizeBase& size) const{ BoxSizeBase ret=*this; return ret-=size; }
	//
	T size() const{ return w*h; }
};


typedef BoxSizeBase<long> CBoxSize;
typedef BoxSizeBase<double> CBoxSizeF;


} //namespace math

//using namespace math;


