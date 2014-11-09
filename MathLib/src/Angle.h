#pragma once

#include "_fwd.h"

namespace math{

//!角度クラステンプレート。座標系は X軸→ Y軸↑
/*!
@param T 内部値の型
@param U 一回転の値。ただし特別に、U=1 は U=PI*2 とみなす
*/
template <class T,int U>
class AngleBase{
private:
	typedef AngleBase<T,U> Me;
public:
	//!1回転の値を取得
	static double UNIT()
	{
		if(U==1)return PI*2;
		else return U;
	}
public:
	//コンストラクタ・デストラクタ
	AngleBase(T _v=0) : v(_v) { }
	template <class T2,int U2> explicit AngleBase(const AngleBase<T2,U2>& rhs);
	//算術演算子
	AngleBase& operator += (const Me& rhs){ v=(T)(v+rhs.v); return *this; }
	AngleBase& operator -= (const Me& rhs){ v=(T)(v-rhs.v); return *this; }
	AngleBase& operator *= (double n){ v=(T)(v*n); return *this; }
	AngleBase& operator /= (double n){ v=(T)(v/n); return *this; }
	//比較演算子
	bool operator == (const Me& n){ return v==n; }
	bool operator != (const Me& n){ return v!=n; }
	//値取り出し
	T value() const{ return v; }
	operator T() const{ return v; }
	//ベクトル変換
	math::CVec2F toVec() const;
private:
	T v;
};

//1回転=2π[rad]
typedef AngleBase<double,1> Rad;

//1回転=65536[idx]
typedef AngleBase<u16,65536> Idx;

//標準型
typedef Rad Angle;

} //namespace math

//using namespace math;

