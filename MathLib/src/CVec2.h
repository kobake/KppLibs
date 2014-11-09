#pragma once

namespace math{

template <class T> class RectBase;

template <class T>
class Vec2Base{
public:
	T x;
	T y;
public:
	//�R���X�g���N�^
	Vec2Base() : x(0), y(0) { }
	Vec2Base(T _x,T _y) : x(_x), y(_y) { }
	template <class T2>
        Vec2Base(const Vec2Base<T2>& rhs){ x=(T)rhs.x; y=(T)rhs.y; }
	//���
	void Assign(T _x,T _y){ x=_x; y=_y; }
	void set(T _x,T _y){ x=_x; y=_y; }
	Vec2Base& operator = (const Vec2Base& rhs){ x=rhs.x; y=rhs.y; return *this; }
	//���Z�q
	Vec2Base& operator *= (T n) { x*=n; y*=n; return *this; }
	Vec2Base& operator /= (T n) { x/=n; y/=n; return *this; }
	Vec2Base& operator += (const Vec2Base& rhs){ x+=rhs.x; y+=rhs.y; return *this; }
	Vec2Base& operator -= (const Vec2Base& rhs){ x-=rhs.x; y-=rhs.y; return *this; }
	//���Z�q���b�p�[
	const Vec2Base operator * (T n) const{ Vec2Base ret=*this; return ret*=n; }
	const Vec2Base operator / (T n) const{ Vec2Base ret=*this; return ret/=n; }
	const Vec2Base operator + (const Vec2Base& rhs) const{ Vec2Base ret=*this; return ret+=rhs; }
	const Vec2Base operator - (const Vec2Base& rhs) const{ Vec2Base ret=*this; return ret-=rhs; }
	const Vec2Base operator - (){ Vec2Base ret=*this; return ret*=-1; }
	//��r
	bool eq(const Vec2Base& pt) const { return x==pt.x && y==pt.y; }
	bool operator == (const Vec2Base& pt) const { return eq(pt); }
	bool operator != (const Vec2Base& pt) const { return !eq(pt); }
	//����
	bool empty() const{ return x==0 && y==0; }
	//��Βl
	T length() const;
	T lengthSq() const;
	//���K��
	Vec2Base& normalize();
	Vec2Base normalized() const{ Vec2Base ret=*this; return ret.normalize(); }
	//��]
	void rotate(float angle);
	//�@��
	Vec2Base normal() const;
};

//�O�����Z�q
template <class T,class N>
Vec2Base<T> operator * (N n,const Vec2Base<T>& pt){ return pt*(T)n; }

//���� ( inner product / dot product )
template <class T>
T dot_product(const Vec2Base<T>& v0,const Vec2Base<T>& v1);

//�O�� ( outer product / cross product )
template <class T>
T cross_product(const Vec2Base<T>& v0,const Vec2Base<T>& v1);


typedef Vec2Base<long> CVec2;
typedef Vec2Base<double> CVec2F;
typedef CVec2 CPoint;

} //namespace math
