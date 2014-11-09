#pragma once

#include "sys/math2/MathVec2.h"

template <int N> class CompassBase{
public:
	typedef CompassBase<N> Me;
protected:
	//なぜか explicit が使えない…
	CompassBase<N>(int _d) : d(_d) { }
public:
	//コンストラクタ・デストラクタ
	CompassBase<N>() : d(0) { } //デフォルトで左
	CompassBase<N>(const Me& rhs) : d(rhs.d) { }
	//演算子
	bool operator == (const Me& rhs) const{ return d==rhs.d; }
	bool operator != (const Me& rhs) const{ return d!=rhs.d; }
	Me& operator = (const Me& rhs){ d=rhs.d; return *this; }
	//方向変換
	Me leftHand() const{ return Me((d-1+4)%4); }
	Me rightHand() const{ return Me((d+1+4)%4); }
	Me backWard() const{ return Me((d+2+4)%4); }
	//内部変数
	int value() const{ return d; }
	//ツール
	MathVec2 getVec() const
	{
		switch(d){
		case 0: return MathVec2(-1, 0);
		case 1: return MathVec2(0, -1);
		case 2: return MathVec2(1, 0);
		case 3: return MathVec2(0, 1);
		}
		return MathVec2(0,0);
	}
	f32 getYaw() const
	{
		return (f32)((2-d)*PI/2);
	}
	//ファクトリー
	static Me create(int _d){ return CompassBase<N>((_d+4+4)%4); }
private:
	int d;
};

//絶対方角
class Compass : public CompassBase<0>{
protected:
	//なぜか explicit が使えない…
	Compass(int _d) : CompassBase<0>(_d) { }
public:
	//コンストラクタ・デストラクタ
	Compass() : CompassBase<0>(0) { } //デフォルトで左
	Compass(const Me& rhs) : CompassBase<0>(rhs) { }
	//デバッグ
	const char* getName() const;
public:
	//定数
	static const Compass West		;
	static const Compass North		;
	static const Compass East		;
	static const Compass South		;
	static const Compass NoCompass	;
};

//相対方角
class Course : public CompassBase<1>{
protected:
	//なぜか explicit が使えない…
	Course(int _d) : CompassBase<1>(_d) { }
public:
	//コンストラクタ・デストラクタ
	Course() : CompassBase<1>(0) { } //デフォルトで左
	Course(const Me& rhs) : CompassBase<1>(rhs) { }
public:
	//定数
	static const Course Backward	; //後方
	static const Course Lefthand	; //左手方向
	static const Course Forward		; //前方
	static const Course Righthand	; //右手方向
	static const Course NoCourse	;
};

