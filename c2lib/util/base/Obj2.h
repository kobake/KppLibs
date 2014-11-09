#pragma once

#include "math/CVec2.h"

//!2次元座標を取得するインターフェース
class ConstObj2{
public:
	typedef Vec2Base<double> t_Vec2;
public:
	virtual ~ConstObj2(){}
	//
	virtual void getPos(t_Vec2* v) const=0;
	t_Vec2 getPos() const{ t_Vec2 v; getPos(&v); return v; }
};

//!2次元座標を取得、設定するインターフェース
class Obj2 : public ConstObj2{
public:
	virtual void setPos(const t_Vec2& v)=0;
};
