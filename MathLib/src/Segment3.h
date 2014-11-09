#pragma once

#include "Vec3.h"

namespace math{

//線分を定義
//segment line
class Segment3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//コンストラクタ・デストラクタ
	Segment3();                                  //初期化無し
	Segment3(const t_Vec3& p0,const t_Vec3& p1); //任意の2点により定義
	Segment3(const t_Vec3* p);                   //任意の2点(配列)により定義
	//インターフェース
	const t_Vec3& getBegin() const{ return pos[0]; }
	const t_Vec3& getEnd() const{ return pos[1]; }
private:
	t_Vec3 pos[2];
};




} //namespace math
