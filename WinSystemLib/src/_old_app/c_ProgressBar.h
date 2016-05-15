class ProgressBar;

#ifndef DEF_PROGRESS_BAR
#define DEF_PROGRESS_BAR

#include "c_Window.h"

class ProgressBar : public Window{
public:
	ProgressBar(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~ProgressBar();
	void setRange(int min,int max);
	void setPos(int pos);
};





#endif
