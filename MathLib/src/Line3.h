#pragma once

#include "Vec3.h"

namespace math{

//直線を定義
class Line3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//生成
	static void constructByPosVector(Line3* line,const t_Vec3& p0,const t_Vec3& v); //任意の点と方向ベクトルにより定義
	static void constructByPosPos(Line3* line,const t_Vec3& p0,const t_Vec3& p1);   //任意の2点により定義
	static void constructByPosPos(Line3* line,const t_Vec3* p);                     //任意の2点(配列)により定義
private:
	void _check() const;
public:
	//コンストラクタ・デストラクタ
	Line3();                                  //初期化無し
	Line3(const t_Vec3& p0,const t_Vec3& v);  //任意の点と方向ベクトルにより定義
	Line3(const Line3& rhs);
	Line3& operator = (const Line3& rhs);
	//取得
	const t_Vec3& getPosSample() const{ return pos0; }
	const t_Vec3& getDirection() const{ return dir; }
private:
	t_Vec3 pos0;
	t_Vec3 dir;
};



} //namespace math
