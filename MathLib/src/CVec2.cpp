#include "_required.h"
#include "CVec2.h"
#include "CRect.h"
#include <cmath>

namespace math{

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        Vec2Base                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//��Βl
template <class T>
T Vec2Base<T>::length() const
{
	return (T)sqrt((double)(x*x+y*y));
}

template <class T>
T Vec2Base<T>::lengthSq() const
{
	return x*x+y*y;
}

//���K��
template <class T>
Vec2Base<T>& Vec2Base<T>::normalize()
{
	T len=length();
	x/=len;
	y/=len;
	return *this;
}

//�@��
template <class T>
Vec2Base<T> Vec2Base<T>::normal() const
{
	return Vec2Base<T>(-y,x); //�f�J���g���W�n�ɂ����č���90����]
}

//��]
template <class T>
void Vec2Base<T>::rotate(float angle)
{
	float new_x=x*cos(angle)-y*sin(angle);
	float new_y=x*sin(angle)+y*cos(angle);
	x=new_x;
	y=new_y;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        �O���֐�                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//���� ( inner product / dot product )
template <class T>
T dot_product(const Vec2Base<T>& v0,const Vec2Base<T>& v1)
{
	return v0.x*v1.x+v0.y*v1.y;
}

//�O�� ( outer product / cross product )
template <class T>
T cross_product(const Vec2Base<T>& v0,const Vec2Base<T>& v1)
{
	return v0.x*v1.y-v0.y*v1.x;
}

// -- -- �����I�ȃC���X�^���X�� -- -- //
template long dot_product(const Vec2Base<long>& v0,const Vec2Base<long>& v1);
template long cross_product(const Vec2Base<long>& v0,const Vec2Base<long>& v1);
template double dot_product(const Vec2Base<double>& v0,const Vec2Base<double>& v1);
template double cross_product(const Vec2Base<double>& v0,const Vec2Base<double>& v1);



} //namespace math
