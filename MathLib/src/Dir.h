#pragma once

#include "_fwd.h"

//!方向クラス。座標系は X軸→ Y軸↓
class Dir{
public:
	//コンストラクタ・デストラクタ
	Dir() : d(0) { } //デフォルトで左
	Dir(const Dir& rhs) : d(rhs.d) { }
	explicit Dir(int _d) : d(_d) { }
	explicit Dir(const math::CVec2& v);
	//演算子
	bool operator == (const Dir& rhs) const{ return d==rhs.d; }
	bool operator != (const Dir& rhs) const{ return d!=rhs.d; }
	Dir& operator = (const Dir& rhs){ d=rhs.d; return *this; }
	//方向変換
	Dir leftHand() const{ return Dir((d-1+4)%4); }
	Dir rightHand() const{ return Dir((d+1+4)%4); }
	Dir backWard() const{ return Dir((d+2+4)%4); }
	//内部変数
	int value() const{ return d; }
	//変換
	math::CVec2 toVec() const;
private:
	int d;
public:
	//定数
};

//インライン展開されることを祈って…
#define DIR_NONE Dir(-1)
#define DIR_L Dir(0)
#define DIR_U Dir(1)
#define DIR_R Dir(2)
#define DIR_D Dir(3)

/*
extern const Dir DIR_NONE;
extern const Dir DIR_L;
extern const Dir DIR_U;
extern const Dir DIR_R;
extern const Dir DIR_D;
*/
