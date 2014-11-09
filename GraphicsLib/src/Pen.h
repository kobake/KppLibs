#pragma once

#include "CColor.h"

class Pen{
public:
	//�萔
	enum Style{
		Normal,
		Inside,
	};
	//���
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
	//�R���X�g���N�^�E�f�X�g���N�^
	Pen(){}
	virtual ~Pen(){}
	//�ݒ�
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
	//�擾
	const State& getState() const{ return state; }
	//�X�V
	virtual void _update()=0;
private:
	State state;
};
