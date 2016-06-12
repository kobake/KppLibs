#pragma once

class Window;

class ScrollManager{
protected:
	Window *wnd;
	int sb_kind;
	SCROLLINFO si;
public:
	ScrollManager(Window *_wnd,int _sb_kind);
	virtual ~ScrollManager();
	void init(Window *_wnd,int _sb_kind);
	LRESULT onScroll(UINT msg,WPARAM wParam,LPARAM lParam);
	//ê›íË
	void setRange(int range_min,int range_max);
	void setPageSize(int page_size);
	//égóp
	int getPos();
	void setPos(int pos);
	void enable(bool e);
};

