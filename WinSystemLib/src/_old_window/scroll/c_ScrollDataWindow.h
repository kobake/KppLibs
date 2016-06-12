#pragma once

#define MOUSE_LEFT	-1
#define MOUSE_RIGHT	1
#define MOUSE_NONE	0

#define CURSOR_SIMPLE	0
#define CURSOR_INVERT	1

#include <BaseLib.h> // t_Table2D
#include <GraphicsLib.h> //Pen
#include "../c_Window.h"

class ScrollManager;
class RectCursor;

class ScrollDataWindow : public Window{
protected:
	virtual bool isValidData()=0;
	virtual int getDataNW()=0;
	virtual int getDataNH()=0;
	virtual void drawData(CDcGraphics *g,int xoff,int yoff)=0;
protected:
	//概観
	Pen::State cursor_pen_param;
	//サイズ関連
	ScrollManager *hscroll;
	ScrollManager *vscroll;
	//操作関連
	RectCursor *cursor;
	int mouse_capture;
	int cursor_begin_x,cursor_begin_y;
	int cursor_figure;
	//オブジェクトとメッセージ
	UINT WM_CURSORBEGIN;
	UINT WM_CURSOREND;
public:
	//コンストラクタ・デストラクタ
	void _init_v();
	ScrollDataWindow(DWORD ex_style,DWORD style,const wstring& caption,int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~ScrollDataWindow();
	//概観設定
	void setCursorPenParam(const Pen::State& _pen_param);
	//カーソル位置設定
	bool setCursorPos(int index);
	bool setCursorPos(int nx,int ny);
	void clearCursor();
	//カーソル位置取得
	IntTable2D *getSelection();
	//マウス動作
	void setMouseMode(int mouse);
	virtual LRESULT onMouse(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onActivate(UINT msg,WPARAM wParam,LPARAM lParam);
	//スクロール
	virtual LRESULT onSize(UINT msg,WPARAM wParam,LPARAM lParam);
	void ScrollDataWindow::realizeScrollRange();
	virtual LRESULT onScroll(UINT msg,WPARAM wParam,LPARAM lParam);
	//描画
	virtual LRESULT onPaint(CDcGraphics *g);
	void setCursorFigure(int figure);
	void drawCursor(CDcGraphics *g,int xoff,int yoff);
	//メッセージ処理
	virtual LRESULT onKey(UINT msg,WPARAM wParam,LPARAM lParam);
	//ツール
	bool ptIn(int nx,int ny);
protected:
	void repaintCursorRect(RectCursor *cursor);
};

