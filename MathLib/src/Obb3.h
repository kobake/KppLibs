#pragma once

#include "Vec3.h"

namespace math{

class Aabb3;
class Intersect3;

//有向境界ボックス (OBB)
class Obb3{
public:
	friend class Intersect3;
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//生成
	static void createByAabb3(Obb3* obb,const Aabb3& aabb);
public:
	//コンストラクタ
	Obb3(const t_Vec3& center,
		const t_Vec3& x_axis,const t_Vec3& y_axis,const t_Vec3& z_axis,
		const t_Vec3& lengths);
	Obb3();
	Obb3(const Obb3& rhs);
	void Assign(const Obb3& rhs);
	Obb3& operator = (const Obb3& rhs){ Assign(rhs); return *this; }
	//取得
	const t_Vec3& center()		const{ return c; }
	const t_Vec3& axis(int i)	const{ return u[i]; }
	const t_Vec3& lengths()		const{ return e; }
	t_Vec3& center()	{ return c; }
	t_Vec3& axis(int i)	{ return u[i]; }
	t_Vec3& lengths()	{ return e; }
	//変換
	void transform(const Mtx<3,4>& mtx);
private:
	t_Vec3 c;    //OBBの中心点                          box center
	t_Vec3 u[3]; //ローカルx,y,z軸                      local x,y,z axis
	t_Vec3 e;    //OBBの各軸に沿って正の幅の半分の範囲  positive half-edge lengths
};


} //namespace math
