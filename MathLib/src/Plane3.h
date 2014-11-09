#pragma once

#include "Vec3.h"

namespace math{

//平面を定義
class Plane3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//コンストラクタ・デストラクタ
	//Plane3(f32 a,f32 b,f32 c,f32 d);                         //ax+by+cz+d=0の平面を定義(未実装)
	Plane3();                                                  //初期化無し
	Plane3(const t_Vec3& p,const t_Vec3& v);                   //任意の点と法線ベクトルにより定義
	Plane3(const t_Vec3& p0,const t_Vec3& p1,const t_Vec3 p2); //任意の3点により定義
	Plane3(t_Vec3* p);                                         //任意の3点(配列)により定義
	Plane3(const Plane3& rhs);
	Plane3& operator = (const Plane3& rhs){ pos0=rhs.pos0; normal=rhs.normal; return *this; }
	//取得
	const t_Vec3& getPosSample() const{ return pos0; }
	const t_Vec3& getNormal() const{ return normal; }
private:
	t_Vec3 pos0;
	t_Vec3 normal;
};


} //namespace math
