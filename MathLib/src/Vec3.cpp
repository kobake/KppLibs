#include "_required.h"
#include <cstdio>
#include <cmath>
#include "Vec3.h"
#include "Mtx.h"
#include "Transformer3.h"

namespace math{

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           基本                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
template <class T>
T Vec3Base<T>::length() const
{
	return (T)sqrt((double)(x*x+y*y+z*z));
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      行列による変換                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

template <class T>
void Vec3Base<T>::transform(const Mtx34& mtx)
{
	//4x4行列に変換
	Mtx44 mtx44;
	mtx34_to_mtx44(&mtx44,mtx);
	//4x4行列の変換を適用
	transform(mtx44);
}

template <class T>
void Vec3Base<T>::transformVector(const Mtx34& mtx)
{
	//4x4行列に変換
	Mtx44 mtx44;
	mtx34_to_mtx44(&mtx44,mtx);
	//4x4行列の変換を適用
	transformVector(mtx44);
}

template <class T>
void Vec3Base<T>::transform(const Mtx44& mtx)
{
	//4行1列行列の用意
	Mtx<4,1> pos;
	pos.at(0,0)=x;
	pos.at(1,0)=y;
	pos.at(2,0)=z;
	pos.at(3,0)=1;
	//行列の乗算
	Mtx<4,1> result=mtxMultiply(mtx,pos);
	//x,y,z設定し直し
	x=(T)result.at(0,0);
	y=(T)result.at(1,0);
	z=(T)result.at(2,0);
}

template <class T>
void Vec3Base<T>::transformVector(const Mtx44& mtx)
{
	Vec3Base<T> zero(0,0,0);
	zero.transform(mtx);
	this->transform(mtx);
	(*this)-=zero;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       高頻度の変換                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//平行移動
template <class T>
Vec3Base<T>& Vec3Base<T>::applyTranslate(const Vec3Base<T>& v)
{
	return (*this)+=v;
}


//回転
template <class T>
Vec3Base<T>& Vec3Base<T>::applyRotate(const Vec3Base<T>& v)
{
	transform(mtxRotate(NULL,v));
	return *this;
}

//スケール
template <class T>
Vec3Base<T>& Vec3Base<T>::applyScale(const Vec3Base<T>& v)
{
	transform(mtxScale(NULL,v));
	return *this;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      ユーティリティ                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

template <class T>
T Vec3Base<T>::getRoll() const
{
	assert(y!=0 || x!=0);
	//XY平面
	return (T)atan2((double)y,(double)x);
}

template <class T>
T Vec3Base<T>::getYaw() const
{
	assert(z!=0 || x!=0);
	//XZ平面
	return (T)atan2((double)-z,(double)x);
}

template <class T>
T Vec3Base<T>::getPitch() const
{
	assert(y!=0 || z!=0);
	//ZY平面
	return (T)atan2((double)y,(double)-z);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         外部関数                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

template <class T>
T dot_product(const Vec3Base<T>& v0,const Vec3Base<T>& v1)
{
	return
		v0.x*v1.x+
		v0.y*v1.y+
		v0.z*v1.z
		;
}

template <class T>
Vec3Base<T> cross_product(const Vec3Base<T>& v0,const Vec3Base<T>& v1)
{
	return Vec3Base<T>(
		v0.y*v1.z-v0.z*v1.y,
		v0.z*v1.x-v0.x*v1.z,
		v0.x*v1.y-v0.y*v1.x
		);
}

template <class T>
void cross_product(Vec3Base<T>* in,const Vec3Base<T>& v0,const Vec3Base<T>& v1)
{
	*in=cross_product(v0,v1);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                   明示的なインスタンス化                    //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
template class Vec3Base<long>;
template class Vec3Base<double>;
template class Vec3Base<float>;

template long dot_product(const Vec3Base<long>& v0,const Vec3Base<long>& v1);
template Vec3Base<long> cross_product(const Vec3Base<long>& v0,const Vec3Base<long>& v1);
template void cross_product(Vec3Base<long>* in,const Vec3Base<long>& v0,const Vec3Base<long>& v1);

template double dot_product(const Vec3Base<double>& v0,const Vec3Base<double>& v1);
template Vec3Base<double> cross_product(const Vec3Base<double>& v0,const Vec3Base<double>& v1);
template void cross_product(Vec3Base<double>* in,const Vec3Base<double>& v0,const Vec3Base<double>& v1);

template float dot_product(const Vec3Base<float>& v0,const Vec3Base<float>& v1);
template Vec3Base<float> cross_product(const Vec3Base<float>& v0,const Vec3Base<float>& v1);
template void cross_product(Vec3Base<float>* in,const Vec3Base<float>& v0,const Vec3Base<float>& v1);



} //namespace math
