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
	//�T��
	Pen::State cursor_pen_param;
	//�T�C�Y�֘A
	ScrollManager *hscroll;
	ScrollManager *vscroll;
	//����֘A
	RectCursor *cursor;
	int mouse_capture;
	int cursor_begin_x,cursor_begin_y;
	int cursor_figure;
	//�I�u�W�F�N�g�ƃ��b�Z�[�W
	UINT WM_CURSORBEGIN;
	UINT WM_CURSOREND;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	void _init_v();
	ScrollDataWindow(DWORD ex_style,DWORD style,const wstring& caption,int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~ScrollDataWindow();
	//�T�ϐݒ�
	void setCursorPenParam(const Pen::State& _pen_param);
	//�J�[�\���ʒu�ݒ�
	bool setCursorPos(int index);
	bool setCursorPos(int nx,int ny);
	void clearCursor();
	//�J�[�\���ʒu�擾
	IntTable2D *getSelection();
	//�}�E�X����
	void setMouseMode(int mouse);
	virtual LRESULT onMouse(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onActivate(UINT msg,WPARAM wParam,LPARAM lParam);
	//�X�N���[��
	virtual LRESULT onSize(UINT msg,WPARAM wParam,LPARAM lParam);
	void ScrollDataWindow::realizeScrollRange();
	virtual LRESULT onScroll(UINT msg,WPARAM wParam,LPARAM lParam);
	//�`��
	virtual LRESULT onPaint(CDcGraphics *g);
	void setCursorFigure(int figure);
	void drawCursor(CDcGraphics *g,int xoff,int yoff);
	//���b�Z�[�W����
	virtual LRESULT onKey(UINT msg,WPARAM wParam,LPARAM lParam);
	//�c�[��
	bool ptIn(int nx,int ny);
protected:
	void repaintCursorRect(RectCursor *cursor);
};

