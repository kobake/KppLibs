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


//!�f�t�H���g�l�̉��ߊ֐�
/*!
value��
  �f�t�H���g�L���ł������� �� �f�t�H���g�l��Ԃ��B
  �����łȂ�������         �� value�����̂܂ܕԂ��B
*/
template <class T>
T def_adjust(T value,T default_value)
{
	if(value==(T)DEF)return default_value;
	else return value;
}
