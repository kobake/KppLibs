#include "_required.h"
#include "Quad2.h"
#include <BaseLib.h>
#include "CRect.h"


namespace math{


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//任意の4点により定義
Quad2::Quad2(const t_Vec2& p0,const t_Vec2& p1,const t_Vec2& p2,const t_Vec2& p3)
{
	pos[0]=p0;
	pos[1]=p1;
	pos[2]=p2;
	pos[3]=p3;
}

//任意の4点(配列)により定義
Quad2::Quad2(const t_Vec2* p)
{
	for(int i=0;i<4;i++){
		pos[i]=p[i];
	}
}

Quad2::Quad2(const Quad2& rhs)
{
	Assign(rhs);
}

Quad2::Quad2()
{
	//ぜんぶ(0,0)
}

void Quad2::Assign(const Quad2& rhs)
{
	for(int i=0;i<4;i++){
		pos[i]=rhs.pos[i];
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         演算子                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Quad2& Quad2::operator += (const t_Vec2& v)
{
	for(int i=0;i<4;i++){
		pos[i]+=v;
	}
	return *this;
}

Quad2& Quad2::operator -= (const t_Vec2& v)
{
	for(int i=0;i<4;i++){
		pos[i]-=v;
	}
	return *this;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         ツール                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//内包判定
bool Quad2::contains(const t_Vec2& v) const
{
	t_Vec2 n,p;
	ValueType dp;
	ValueType dp_sign=0;
	for(int i=0;i<4;i++){
		//辺の法線
		n=(pos[(i+1)%4]-pos[i]).normal();
		//点へのベクトル
		p=v-pos[i];
		//内積
		dp=unit(dot_product(n,p));
		//内積の符号が前回と違ったらfalse
		if(dp_sign==0)dp_sign=dp;
		else if(dp_sign*dp<0){
			return false;
		}
	}
	return true;
}

//交差判定 (※厳密ではない)
bool Quad2::intersects(const Quad2& quad) const
{
	//お互いにどれか1点でも内包していればOK
	for(int i=0;i<4;i++){
		if(this->contains(quad.at(i))){
			return true;
		}
		else if(quad.contains(this->at(i))){
			return true;
		}
	}
	return false;
}



//バウンディングボックスを取得
Quad2::t_Rect Quad2::getBounds() const
{
	t_Vec2 lt=pos[0],rb=pos[0];
	for(int i=1;i<4;i++){
		lt.x=tmin(lt.x,pos[i].x);
		lt.y=tmin(lt.y,pos[i].y);
	}
	return t_Rect(lt,rb);
}


} //namespace math
