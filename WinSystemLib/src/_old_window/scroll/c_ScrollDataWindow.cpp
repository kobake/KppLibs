#include <windows.h>
#include <string>
#include <BaseLib.h>
#include "../../_old_app/c_App.h"
#include "c_ScrollDataWindow.h"
#include "../f_window.h"
#include "../../map/c_RectCursor.h"
#include "c_ScrollManager.h"

using namespace std;
static RectCursor old_cursor;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  �R���X�g���N�^�E�f�X�g���N�^  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void ScrollDataWindow::_init_v()
{
	hscroll=NULL;
	vscroll=NULL;
	cursor=new RectCursor();
	mouse_capture=0;
	cursor_begin_x=0;
	cursor_begin_y=0;
	cursor_figure=0;
	WM_CURSORBEGIN=0;
	WM_CURSOREND=0;
}

ScrollDataWindow::ScrollDataWindow(DWORD ex_style,DWORD style,const wstring& caption,int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_SCROLL_WINDOW,0,_parent,_option,_id)
{
	_init_v();
	setCursorFigure(CURSOR_SIMPLE);
	setMouseMode(MOUSE_NONE);
	//
	convertXYWH(&x,&y,&w,&h);
	hwnd=CreateAWindow(g_app->getInstance(),g_app->getPrevInstance(),L"winScroll",DefWindowProc,(HBRUSH)(COLOR_WINDOW+1),NULL,NULL,
		ex_style,style,caption.c_str(),x,y,w,h,getParent()->getHWND(),NULL);
	_afterCreate();
	hscroll=new ScrollManager(this,SB_HORZ);
	vscroll=new ScrollManager(this,SB_VERT);
	messageNotify(true);
	paintByGraphics(true);
//	onClientSize(-1,-1);
//	realizeScrollRange();
}

ScrollDataWindow::~ScrollDataWindow()
{
	delete cursor;
	delete hscroll;
	delete vscroll;
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- �T�ϐݒ� -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void ScrollDataWindow::setCursorPenParam(const Pen::State& _pen_param)
{
	cursor_pen_param=_pen_param;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  �J�[�\���ʒu�ݒ�  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool ScrollDataWindow::setCursorPos(int index)
{
	if(index<0 || index>=getDataNW()*getDataNH()){
		return false;
	}else{
		old_cursor=*cursor;
		if(cursor->setXYWH(index%getDataNW(),index/getDataNW(),1,1)!=0){
			repaintCursorRect(&old_cursor);
			repaintCursorRect(cursor);
		}
		return true;
	}
}

bool ScrollDataWindow::setCursorPos(int nx,int ny)
{
	if(!ptIn(nx,ny)){
		return false;
	}else{
		old_cursor=*cursor;
		if(cursor->setXYWH(nx,ny,1,1)!=0){
			repaintCursorRect(&old_cursor);
			repaintCursorRect(cursor);
		}
		return true;
	}
}

void ScrollDataWindow::clearCursor()
{
	old_cursor=*cursor;
	if(cursor->setXYWH(0,0,0,0)!=0){
		repaintCursorRect(&old_cursor);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  �J�[�\���ʒu�擾  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//�J�[�\���������S�C���f�b�N�X��Ԃ�
IntTable2D *ScrollDataWindow::getSelection()
{
	static IntTable2D ret(1,1);
	ret.setSize(cursor->w(),cursor->h(),true);
	int i,j;
	for(j=0;j<cursor->h();j++){
		for(i=0;i<cursor->w();i++){
			ret.set(i,j,cursor->x()+i+(cursor->y()+j)*getDataNW());
		}
	}
	return &ret;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- �}�E�X����  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//�}�E�X�̎g�p�{�^���w��
void ScrollDataWindow::setMouseMode(int mouse)
{
	if(mouse==MOUSE_LEFT){
		WM_CURSORBEGIN=WM_LBUTTONDOWN;
		WM_CURSOREND=WM_LBUTTONUP;
	}else if(mouse==MOUSE_RIGHT){
		WM_CURSORBEGIN=WM_RBUTTONDOWN;
		WM_CURSOREND=WM_RBUTTONUP;
	}else{
		WM_CURSORBEGIN=0;
		WM_CURSOREND=0;
	}
}

LRESULT ScrollDataWindow::onMouse(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(!isValidData())return 0L;
	int x=(short int)LOWORD(lParam)+hscroll->getPos()*32; x/=32;
	int y=(short int)HIWORD(lParam)+vscroll->getPos()*32; y/=32;
	if(msg==WM_CURSORBEGIN){
		if(ptIn(x,y)){
			mouse_capture=1;
			SetCapture(hwnd);
			cursor_begin_x=x;
			cursor_begin_y=y;
			old_cursor=*cursor;
			if(cursor->setXYWH(cursor_begin_x,cursor_begin_y,1,1)!=0){
				repaintCursorRect(&old_cursor);
				repaintCursorRect(cursor);
			}
		}
	}else if(msg==WM_CURSOREND){
		mouse_capture=0;
		ReleaseCapture();
	}else if(msg==WM_MOUSEMOVE){
		if(mouse_capture){
			//���W����
			x=adjust(x,0,getDataNW()-1);
			y=adjust(y,0,getDataNH()-1);
			//
			old_cursor=*cursor;
			if(cursor->setXYXY(cursor_begin_x,cursor_begin_y,x,y)!=0){
				repaintCursorRect(&old_cursor);
				repaintCursorRect(cursor);
			}
		}
	}
	return 0L;
}
LRESULT ScrollDataWindow::onActivate(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(wParam==WA_INACTIVE){
		onMouse(WM_LBUTTONUP,0,0);
		onMouse(WM_RBUTTONUP,0,0);
	}
	return 0L;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- �X�N���[�� -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT ScrollDataWindow::onSize(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(!isValidData()){ hscroll->enable(false); vscroll->enable(false); return 0L; }
	Window::onSize(msg,wParam,lParam);
	realizeScrollRange();
	return 0L;
}

void ScrollDataWindow::realizeScrollRange()
{
	if(!isValidData())return;
	int client_nw=rcClient.right / 32;
	int client_nh=rcClient.bottom / 32;
	// -- �X�N���[���o�[�ݒ� -- //
	hscroll->setRange(0,max(0,getDataNW()-client_nw));
	hscroll->setPageSize(1);
	vscroll->setRange(0,max(0,getDataNH()-client_nh));
	vscroll->setPageSize(1);
}

LRESULT ScrollDataWindow::onScroll(UINT msg,WPARAM wParam,LPARAM lParam)
{
	hscroll->onScroll(msg,wParam,lParam);
	vscroll->onScroll(msg,wParam,lParam);
	repaint(false);
	return 0L;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- �`��  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT ScrollDataWindow::onPaint(CDcGraphics *g)
{
	if(!isValidData()){ return Window::onPaint(g); }
	//�`��J�n
	drawData(g,hscroll->getPos(),vscroll->getPos());
	drawCursor(g,hscroll->getPos(),vscroll->getPos());
	return 0L;
}

//�J�[�\���̌�����
void ScrollDataWindow::setCursorFigure(int figure)
{
	cursor_figure=figure;
}

#if 0
//�J�[�\���`��
void ScrollDataWindow::drawCursor(CDcGraphics *g,int xoff,int yoff)
{
	int x=cursor->x()-xoff; x*=32;
	int y=cursor->y()-yoff; y*=32;
	if(cursor_figure==CURSOR_SIMPLE){
		g->getPen()->setState(cursor_pen_param);
		g->drawRect(CRect(x,y,cursor->w()*32,cursor->h()*32));
	}else if(cursor_figure==CURSOR_INVERT){
		bool b=g->setInvert(true);
		g->FillRect(CRect(x,y,cursor->w()*32,cursor->h()*32));
		g->setInvert(b);
	}
}
#endif

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- ���b�Z�[�W����  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT ScrollDataWindow::onKey(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(!isValidData())return 0L;
	if(msg==WM_KEYDOWN){
		switch(wParam){
		case VK_LEFT:	SendMessage(hwnd,WM_HSCROLL,MAKEWPARAM(SB_LINELEFT, 0),(LPARAM)NULL); break;
		case VK_RIGHT:	SendMessage(hwnd,WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,0),(LPARAM)NULL); break;
		case VK_UP:		SendMessage(hwnd,WM_VSCROLL,MAKEWPARAM(SB_LINEUP,   0),(LPARAM)NULL); break;
		case VK_DOWN:	SendMessage(hwnd,WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN, 0),(LPARAM)NULL); break;
		}
	}
	return 0L;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --  �c�[��  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool ScrollDataWindow::ptIn(int nx,int ny)
{
	return nx>=0 && nx<getDataNW() && ny>=0 && ny<getDataNH();
}

void ScrollDataWindow::repaintCursorRect(RectCursor *cursor)
{
	repaint((cursor->x()-hscroll->getPos())*32, (cursor->y()-vscroll->getPos())*32, cursor->w()*32, cursor->h()*32,false);
}


