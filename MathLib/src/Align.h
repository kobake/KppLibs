#pragma once

namespace math{

template <class T> class Vec2Base;
template <class T> class BoxSizeBase;

//”z’u
enum EAlignValue{
	X_LEFT		=(1<<0),
	X_CENTER	=(1<<1),
	X_RIGHT		=(1<<2),
	Y_TOP		=(1<<3),
	Y_CENTER	=(1<<4),
	Y_BOTTOM	=(1<<5),
};
inline EAlignValue operator | (EAlignValue lhs,EAlignValue rhs){ return (EAlignValue)((int)lhs | (int)rhs); }


class Align{
public:
	Align(EAlignValue _align) : align(_align) { }

	template <class T>
	T adjustX(T x,T w) const;

	template <class T>
	T adjustY(T y,T h) const;

	template <class T>
	Vec2Base<T> adjust(const Vec2Base<T>& pos,const BoxSizeBase<T>& size) const;

	bool operator == (EAlignValue e) const{ return e == align; }
private:
	EAlignValue align;
};


} //namespace math

