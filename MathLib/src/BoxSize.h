#pragma once

namespace math{

template <class T> class RectBase;

template <class T>
class BoxSizeBase{
private:
	typedef RectBase<T> t_Rect;
public:
	//※WindowsSDK の SIZE と同じ構造にしておく (念のため)
	T w;
	T h;
public:
	//コンストラクタ・デストラクタ
	explicit BoxSizeBase(const t_Rect& rc);
	BoxSizeBase(T _w=0,T _h=0) : w(_w), h(_h) { }
	//代入
	void Assign(T _w,T _h){ w=_w; h=_h; }
	void set(T _w,T _h){ w=_w; h=_h; }
	//比較
	bool eq(const BoxSizeBase& right) const{ return w==right.w && h==right.h; }
	//比較演算子
	bool operator == (const BoxSizeBase& right) const{ return eq(right); }
	bool operator != (const BoxSizeBase& right) const{ return !eq(right); }
	//演算子
	BoxSizeBase& operator *= (T n){ w*=n; h*=n; return *this; }
	BoxSizeBase& operator /= (T n){ w/=n; h/=n; return *this; }
	//演算子ラッパー
	const BoxSizeBase operator * (T n) const{ BoxSizeBase ret=*this; return ret*=n; }
	const BoxSizeBase operator / (T n) const{ BoxSizeBase ret=*this; return ret/=n; }
	//演算子
	BoxSizeBase& operator += (const BoxSizeBase& size){ w+=size.w; h+=size.h; return *this; }
	BoxSizeBase& operator -= (const BoxSizeBase& size){ w-=size.w; h-=size.h; return *this; }
	//演算子ラッパー
	const BoxSizeBase operator + (const BoxSizeBase& size) const{ BoxSizeBase ret=*this; return ret+=size; }
	const BoxSizeBase operator - (const BoxSizeBase& size) const{ BoxSizeBase ret=*this; return ret-=size; }
	//
	T size() const{ return w*h; }
};


typedef BoxSizeBase<long> CBoxSize;
typedef BoxSizeBase<double> CBoxSizeF;


} //namespace math

//using namespace math;


