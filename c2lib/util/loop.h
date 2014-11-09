#pragma once

//※範囲 [min_value, max_value)
template <class T> T loop_adjust(T t,T min_value,T max_value)
{
	while(t<min_value)t+=(max_value-min_value);
	while(t>=max_value)t-=(max_value-min_value);
	return t;
}

template <class T> class loop{
public:
	loop(T _min_value,T _max_value) : min_value(_min_value), max_value(_max_value) { }
	T adjust(T t)
	{
		return loop_adjust(t,min_value,max_value);
	}
	bool contain(T t) const
	{
		return t>=min_value && t<max_value;
	}
private:
	T min_value;
	T max_value;
};

#include <limits>
using namespace std;

template <class T> class loop_value{
public:
	loop_value()
	{
		value=T();
		min_value=numeric_limits<T>::denorm_min();
		max_value=numeric_limits<T>::max();
	}
	loop_value(T _value,T _min_value,T _max_value)
	{
		reset(_value,_min_value,_max_value,_loop);
	}
	//型変換
	operator T() const
	{
		return value;
	}
	//演算子
	loop_value<T>& operator = (T t)
	{
		value=loop_adjust(t,min_value,max_value);
		return *this;
	}
	loop_value<T>& operator += (T t)
	{
		value=loop_adjust(value+t,min_value,max_value);
		return *this;
	}
	loop_value<T>& operator -= (T t)
	{
		value=loop_adjust(value-t,min_value,max_value);
		return *this;
	}
	loop_value<T>& operator ++ () //前置
	{
		return operator += (1);
	}
	loop_value<T>& operator -- () //前置
	{
		return operator -= (1);
	}
	T operator ++ (T) //後置
	{
		T ret=value;
		operator += (1);
		return ret;
	}
	T operator -- (T) //後置
	{
		T ret=value;
		operator -= (1);
		return ret;
	}
	//範囲再設定
	void reset(T _value,T _min_value,T _max_value)
	{
		min_value=_min_value;
		max_value=_max_value;
		value=loop_adjust(_value,min_value,max_value);
	}
private:
	T value;
	T min_value;
	T max_value;
};


