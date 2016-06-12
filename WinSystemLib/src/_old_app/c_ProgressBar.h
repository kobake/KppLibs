#pragma once

#include "../_old_window/c_Window.h"

class ProgressBar : public Window{
public:
	ProgressBar(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~ProgressBar();
	void setRange(int min,int max);
	void setPos(int pos);
};
