#pragma once

#include "CColor.h"

class Pen{
public:
	//定数
	enum Style{
		Normal,
		Inside,
	};
	//状態
	struct State{
		CColor color;
		int width;
		Style style;

		State()
		{
			color.set(0,0,0);
			width=1;
			style=Normal;
		}
		bool operator == (const State& rhs) const
		{
			return color==rhs.color && width==rhs.width && style==rhs.style;
		}
		bool operator != (const State& rhs) const
		{
			return !operator==(rhs);
		}
	};
public:
	//コンストラクタ・デストラクタ
	Pen(){}
	virtual ~Pen(){}
	//設定
	void SetColor(const CColor& _color)
	{
		if(state.color!=_color){
			state.color=_color;
			_update();
		}
	}
	void setWidth(int _width)
	{
		if(state.width!=_width){
			state.width=_width;
			_update();
		}
	}
	void setStyle(Style _style)
	{
		if(state.style!=_style){
			state.style=_style;
			_update();
		}
	}
	void setState(const State& _state)
	{
		if(state!=_state){
			state=_state;
			_update();
		}
	}
	//取得
	const State& getState() const{ return state; }
	//更新
	virtual void _update()=0;
private:
	State state;
};
