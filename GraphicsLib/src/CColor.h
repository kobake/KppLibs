#pragma once

#include "sdk/libfwd_win.h"

enum ColorType{
	Win,
	Java,
};


class CColor{
public:
	static COLORREF s_turn(COLORREF c)
	{
		return ((c&0xFF)<<16) | (c&0xFF00) | ((c&0xFF0000)>>16);
	}
public:
	CColor()
	{
		set(0,0,0);
	}
	CColor(int r,int g,int b)
	{
		set(r,g,b);
	}
	CColor(const CColor& rhs)
	{
		set(rhs.r,rhs.g,rhs.b);
	}
	CColor(int color,ColorType type)
	{
		if(type==Java){
			w_color=s_turn(color);
		}else{
			w_color=color;
		}
	}
	CColor& operator = (const CColor& rhs)
	{
		set(rhs.r,rhs.g,rhs.b);
		return *this;
	}
	void set(int r,int g,int b)
	{
		w_color=((r&0xFF)<<0) | ((g&0xFF)<<8) | ((b&0xFF)<<16);
	}
	//比較
	bool equals(const CColor& rhs) const
	{
		return this->w_color==rhs.w_color;
	}
	bool operator == (const CColor& rhs) const{ return equals(rhs); }
	bool operator != (const CColor& rhs) const{ return !equals(rhs); }
	//計算
	CColor& operator *= (double n);
	CColor& operator /= (double n);
	CColor operator * (double n) const{ CColor ret=*this; return ret*=n; }
	CColor operator / (double n) const{ CColor ret=*this; return ret/=n; }
	//各値
//	int r() const{ return (w_color>> 0)&0xFF; }
//	int g() const{ return (w_color>> 8)&0xFF; }
//	int b() const{ return (w_color>>16)&0xFF; }
	//ネイティブに変換
	COLORREF win_color() const{ return w_color; }
	RGBQUAD win_color2() const;
	int java_color() const{ return s_turn(w_color); }
public:
	//Windows形式で保持しておく
	union{
		COLORREF w_color;
		struct{
			byte r;
			byte g;
			byte b;
			byte a;
		};
	};
};
