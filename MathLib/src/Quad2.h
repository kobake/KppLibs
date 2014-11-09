#pragma once

#include "CVec2.h"

namespace math{

/*
class Polygon2{
public:
	//左回りに定義
	//3つ以上の任意点により定義
	Polygon2(const t_Vec2* points,int npoints);
	//取得
	size_t size();
	t_Vec2 at(int index);
	//ツール
	CRect getBounds() const; //バウンディングボックスを取得
private:

};
*/

class Quad2{
public:
	typedef double ValueType;
	typedef Vec2Base<ValueType> t_Vec2;
	typedef RectBase<ValueType> t_Rect;
public:
	//左回りに定義
	Quad2(const t_Vec2& p0,const t_Vec2& p1,const t_Vec2& p2,const t_Vec2& p3); //任意の4点により定義
	Quad2(const t_Vec2* p);                                                     //任意の4点(配列)により定義
	Quad2(const Quad2& rhs);
	Quad2();
	void Assign(const Quad2& rhs);
	Quad2& operator = (const Quad2& rhs){ Assign(rhs); return *this; }
	//取得
	size_t size() const{ return 4; }
	const t_Vec2& at(int i) const{ return pos[i]; }
	t_Vec2& at(int i){ return pos[i]; }
	//演算子
	Quad2& operator += (const t_Vec2& v);
	Quad2& operator -= (const t_Vec2& v);
	//演算子ラップ
	Quad2 operator + (const t_Vec2& v) const{ Quad2 ret=*this; return ret+=v; }
	Quad2 operator - (const t_Vec2& v) const{ Quad2 ret=*this; return ret-=v; }
	//ツール
	bool contains(const t_Vec2& v) const; //内包判定
	bool intersects(const Quad2& quad) const; //交差判定
	t_Rect getBounds() const; //バウンディングボックスを取得
private:
	t_Vec2 pos[4];
};


} //namespace math
