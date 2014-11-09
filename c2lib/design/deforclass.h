#pragma once

class Def{
public:
//	operator f32() const{ return (f32)(short)(0x7FFF); }
	operator int() const{ return (int)(0x6fffffff); }
};

template <class T> class DefOrClass{
public:
	DefOrClass(const T& _t) : t(&_t) { }
	DefOrClass(const Def&) : t(NULL) { }
	const T& get() const{ return *t; }
	bool isDef() const{ return t==NULL; }
	//
	operator const T&() const{ return *t; }
private:
	const T* t;
};

#define DEF Def()


//!デフォルト値の解釈関数
/*!
valueが
  デフォルト記号であったら → デフォルト値を返す。
  そうでなかったら         → valueをそのまま返す。
*/
template <class T>
T def_adjust(T value,T default_value)
{
	if(value==(T)DEF)return default_value;
	else return value;
}
