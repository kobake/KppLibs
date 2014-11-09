#include "_required.h"
#include "Aabb3.h"
#include "util/std.h"
#include "CRect.h"
#include "Mtx.h"


namespace math{

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           生成                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Aabb3::createByMinMax(Aabb3* aabb,const t_Vec3& min_pos,const t_Vec3& max_pos)
{
	aabb->setMinMax(min_pos,max_pos);
}

void Aabb3::createByCenterAndLengths(Aabb3* aabb,const t_Vec3& center,const t_Vec3& half_lengths)
{
	aabb->setCenterAndLengths(center,half_lengths);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
/*
Aabb3::Aabb3(const t_Vec3& center,const t_Vec3& half_lengths)
: c(center), r(half_lengths)
{
}
*/
void Aabb3::_check() const
{
	assert(r.x>=0);
	assert(r.y>=0);
	assert(r.z>=0);
}

Aabb3::Aabb3()
{
	//ぜんぶ(0,0)
}

Aabb3::Aabb3(const Aabb3& rhs)
{
	Assign(rhs);
}

void Aabb3::Assign(const Aabb3& rhs)
{
	c=rhs.c;
	r=rhs.r;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           設定                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Aabb3::setCenterAndLengths(const t_Vec3& center,const t_Vec3& half_lengths)
{
	c=center;
	r=half_lengths; //すべて正
	_check();
}

void Aabb3::setMinMax(const t_Vec3& min_pos,const t_Vec3& max_pos)
{
	c=(min_pos+max_pos)/2;
	r=(max_pos-min_pos)/2; //すべて正
	_check();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           結合                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Aabb3::merge(const Aabb3& rhs)
{
	t_Vec3 this_min=getMin();
	t_Vec3 this_max=getMax();
	t_Vec3 rhs_min=rhs.getMin();
	t_Vec3 rhs_max=rhs.getMax();
	t_Vec3 new_min=t_Vec3(
		tmin(this_min.x,rhs_min.x),
		tmin(this_min.y,rhs_min.y),
		tmin(this_min.z,rhs_min.z)
		);
	t_Vec3 new_max=t_Vec3(
		tmax(this_max.x,rhs_max.x),
		tmax(this_max.y,rhs_max.y),
		tmax(this_max.z,rhs_max.z)
		);
	createByMinMax(this,new_min,new_max);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           変換                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void Aabb3::transform(const Mtx<3,4>& mtx)
{
	//8通りの頂点について検証する
	t_Vec3 p;
	t_Vec3 min_p;
	t_Vec3 max_p;
	for(int i=0;i<8;i++){
		p.x=c.x+r.x*(((i>>2)&1)?-1:1);
		p.y=c.y+r.y*(((i>>1)&1)?-1:1);
		p.z=c.z+r.z*(((i>>0)&1)?-1:1);
		p.transform(mtx);
		//
		if(i==0){
			min_p=p;
			max_p=p;
		}else{
			min_p.x=tmin(min_p.x,p.x);
			min_p.y=tmin(min_p.y,p.y);
			min_p.z=tmin(min_p.z,p.z);
			max_p.x=tmax(max_p.x,p.x);
			max_p.y=tmax(max_p.y,p.y);
			max_p.z=tmax(max_p.z,p.z);
		}
	}
	createByMinMax(this,min_p,max_p);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          ツール                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//交差判定
bool Aabb3::intersects(const Aabb3& aabb) const
{
	//###未実装
	return false;
}

//平行移動(可逆)
void Aabb3::applyTranslate(const t_Vec3& v)
{
	c.applyTranslate(v);
}

//回転    (不可逆)
void Aabb3::applyRotate(const t_Vec3& v)
{
	//8通りの頂点について検証する
	t_Vec3 r2;
	t_Vec3 ret;
	for(int i=0;i<8;i++){
		r2.x=r.x*(((i>>2)&1)?-1:1);
		r2.y=r.y*(((i>>1)&1)?-1:1);
		r2.z=r.z*(((i>>0)&1)?-1:1);
		r2.applyRotate(v);
		//
		if(i==0){
			ret=r2;
		}else{
			ret.x=tmax(ret.x,r2.x);
			ret.y=tmax(ret.y,r2.y);
			ret.z=tmax(ret.z,r2.z);
		}
	}
	r=ret;
}

//スケール(可逆)
void Aabb3::applyScale(const t_Vec3& v)
{
	r.applyScale(v);
}


} //namespace math
