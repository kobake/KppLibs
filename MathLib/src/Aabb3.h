#pragma once

#include "Vec3.h"
#include "Mtx.h"

namespace math{

//軸平行境界ボックス AABB (axis-aligned bounding box)
class Aabb3{
public:
	typedef double ValueType;
	typedef Vec3Base<ValueType> t_Vec3;
public:
	//生成
	static void createByMinMax(Aabb3* aabb,const t_Vec3& min_pos,const t_Vec3& max_pos);
	static void createByCenterAndLengths(Aabb3* aabb,const t_Vec3& center,const t_Vec3& half_lengths);
private:
	void _check() const;
public:
	//コンストラクタ
//	Aabb3(const t_Vec3& center,const t_Vec3& half_lengths);
	Aabb3();
	Aabb3(const Aabb3& rhs);
	void Assign(const Aabb3& rhs);
	Aabb3& operator = (const Aabb3& rhs){ Assign(rhs); return *this; }
	//取得
	const t_Vec3& getCenter() const		{ return c; }
	const t_Vec3& getHalfLengths() const{ return r; }
	t_Vec3 getMin() const{ return c-r; }
	t_Vec3 getMax() const{ return c+r; }
	//設定
	void setCenterAndLengths(const t_Vec3& center,const t_Vec3& half_lengths);
	void setMinMax(const t_Vec3& min_pos,const t_Vec3& max_pos);
	//結合
	void merge(const Aabb3& aabb);
	//変換
	void transform(const Mtx<3,4>& mtx);
	//ツール
	bool intersects(const Aabb3& obb) const; //交差判定
	void applyTranslate(const t_Vec3& v);	 //平行移動(可逆)
	void applyRotate(const t_Vec3& v);		 //回転    (不可逆)
	void applyScale(const t_Vec3& v);		 //スケール(可逆)
private:
	t_Vec3 c;
	t_Vec3 r;
};


} //namespace math
