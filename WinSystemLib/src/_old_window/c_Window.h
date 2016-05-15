#pragma once

#include "common/_required.h"

#include <vector>

#define UNIQUE_ID -2

//1�o�C�g 0x000000FF=1111 1111
//2�o�C�g 0x0000FFFF=1111 1111 1111 1111
//3�o�C�g 0x00FFFFFF=1111 1111 1111 1111 1111 1111
//4�o�C�g 0xFFFFFFFF=1111 1111 1111 1111 1111 1111 1111 1111

// -- -- �{�[�_�[�t���O -- -- //
#define WINDOWF_BORDER(option) ((option) &	0x00030000)
#define WINDOWF_BORDER_NONE					0x00000000	// 0000 0000 0000 0000 | 0000 0000 0000 0000
#define WINDOWF_BORDER_SINGLE				0x00010000	// 0000 0000 0000 0001 | 0000 0000 0000 0000
#define WINDOWF_BORDER_CLIENTEDGE			0x00020000	// 0000 0000 0000 0010 | 0000 0000 0000 0000

// -- -- �^�u�X�g�b�v -- -- //
#define WINDOWF_TAB(option)	((option) &		0x000C0000)
#define WINDOWF_TABSTOP						0x00040000	// 0000 0000 0000 0100 | 0000 0000 0000 0000
#define WINDOWF_NOTABSTOP					0x00080000	// 0000 0000 0000 1000 | 0000 0000 0000 0000

// -- -- �X�N���[�� -- -- //
#define WINDOWF_SCROLL(option) ((option) &	0x00700000)
#define WINDOWF_SCROLL_HORZ					0x00100000	// 0000 0000 0001 0000 | 0000 0000 0000 0000
#define WINDOWF_SCROLL_VERT					0x00200000	// 0000 0000 0010 0000 | 0000 0000 0000 0000
#define WINDOWF_SCROLL_MANAGE				0x00400000	// 0000 0000 0100 0000 | 0000 0000 0000 0000

// -- -- ��{ -- -- //
#define WINDOWF_BASE(option) ((option) &	0x01800000)
#define WINDOWF_BASE_CHILD					0x00800000	// 0000 0000 1000 0000 | 0000 0000 0000 0000
#define WINDOWF_BASE_VISIBLE				0x01000000	// 0000 0001 0000 0000 | 0000 0000 0000 0000

// -- -- �R���g���[���ŗL�t���O -- -- //
#define WINDOWF_CONTROL(option) ((option) &	0x0000FFFF)	// 0000 0000 0000 0000 | 1111 1111 1111 1111

#define GET_WIN_STYLE(option)		((WINDOWF_BASE(option) & WINDOWF_BASE_CHILD)?WS_CHILD:0) | \
									((WINDOWF_BASE(option) & WINDOWF_BASE_VISIBLE)?WS_VISIBLE:0) | \
									((WINDOWF_SCROLL(option) & WINDOWF_SCROLL_HORZ)?WS_HSCROLL:0) | \
									((WINDOWF_SCROLL(option) & WINDOWF_SCROLL_VERT)?WS_VSCROLL:0) | \
									((WINDOWF_BORDER(option) & WINDOWF_BORDER_SINGLE)?WS_BORDER:0)
#define GET_WIN_EX_STYLE(option)	((WINDOWF_BORDER(option) & WINDOWF_BORDER_CLIENTEDGE)?WS_EX_CLIENTEDGE:0)

#include "include_c_message.h"
#include "t_SimpleList.h"

class CGraphics;
class CDcGraphics;

class Icon;
class ScrollManager;
class Window;

struct CHILD{
	int id;
	Window *window;
	static int comp(const CHILD *child1,const CHILD *child2){ return child1->id-child2->id; }
};

void SimpleList_newValue(CHILD &dst);
void SimpleList_copyValue(CHILD &dst,CHILD &src);
void SimpleList_deleteValue(CHILD &dst);

struct NMK{
	NMHDR hdr;
	WPARAM wParam;
	LPARAM lParam;
};

class Window{

// -- -- -- -- Window�N���X���L�ݒ� & �c�[�� -- -- -- -- //
public:
	static void setFontOnCreate(HFONT hFont);
	static HFONT getFontOnCreate();
	static Window *getNullWindow();
	static void convertXYWH(int *x,int *y,int *w,int *h);

// -- -- -- -- Window�N���X�J�n -- -- -- -- //
private:
	int id;
	int option;
	Window *parent;
protected:
	//�C���X�^���X�쐬
	int window_type;
	HWND hwnd;
	WNDPROC WndProcWindowDef;
	LPARAM userdata; //���[�U�[��`�l
	//�E�B���h�E���
	RECT rcWindow;
	RECT rcClient;
	ScrollManager *hscroll;
	ScrollManager *vscroll;
	//���b�Z�[�W�ʒm
	bool notify_drop;
	bool notify_message;
	bool g_paint;
	//�e�q
	SimpleList<CHILD> children;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	//Window(int _type=0,LPARAM _lParam=0,Window *_parent=NULL,int _option=0,int _id=UNIQUE_ID);
	Window(int _type,LPARAM _lParam,Window *_parent,int _option,int _id=UNIQUE_ID);
	virtual ~Window();
protected:
	void _afterCreate(); //�������g�p�i���ɁA�h����ł̂݁j
	int _addChild(Window *_child,int _id);
	int _addChild(Window *_child);
public:
	//��{���擾�E�ݒ�
	HWND getHWND(){ return hwnd; }
	int getID(){ return id; }
	bool isNullWindow(){ return window_type==-1; }
	int getType(){ return window_type; }
	void setParam(LPARAM _lParam){ userdata=_lParam; }
	Window *getParent(){ return parent; }
	//���b�Z�[�W�ʒm
	void messageNotify(bool _notify){ notify_message=_notify; }
	void paintByGraphics(bool _g_paint){ g_paint=_g_paint; }

	//�ʒu�E�T�C�Y���f
	void onWindowSize(int x,int y,int w,int h);
	void onClientSize(int w,int h);

	//�ʒu�E�T�C�Y�擾
	const RECT *getWindowRect(){ return &rcWindow; }
	int getLeft()	{	return rcWindow.left;					}
	int getTop()	{	return rcWindow.top;					}
	int getRight()	{	return rcWindow.right;					}
	int getBottom()	{	return rcWindow.bottom;					}
	int getWidth()	{	return rcWindow.right-rcWindow.left;	}
	int getHeight()	{	return rcWindow.bottom-rcWindow.top;	}
	const RECT *getClientRect(){ return &rcClient; }
	int getClientLeft()		{	return rcClient.left;					}
	int getClientTop()		{	return rcClient.top;					}
	int getClientRight()	{	return rcClient.right;					}
	int getClientBottom()	{	return rcClient.bottom;					}
	int getClientWidth()	{	return rcClient.right-rcClient.left;	}
	int getClientHeight()	{	return rcClient.bottom-rcClient.top;	}

	//�ʒu�E�T�C�Y�ݒ�
	virtual void move(int x,int y,int w,int h);
	void setClientSize(int w,int h);
	void moveInDesktopClient();

	//�\�����
	void hide();
	void show(int nCmdShow=SW_SHOW);
	void showTop(int nCmdShow=SW_SHOW);
	bool isVisible();
	void setEnable(bool e){ EnableWindow(hwnd,e?TRUE:FALSE); }
	bool getEnabled(){ return IsWindowEnabled(hwnd)!=0; }
	void toFront(){ BringWindowToTop(hwnd); }

	//�e�L�X�g
	void setFont(HFONT hFont);
	void setText(const wstring& text);
	void setTextF(const wchar* text,...);
	wchar *getTextTemp();
	wchar *getTextAlloc();
	int getTextLength(){ return GetWindowTextLength(hwnd); }

	//�X�N���[��
	void setScrollRange(int range_min,int range_max,int sb_kind);
	void setScrollPos(int pos,int sb_kind);
	int getScrollPos(int sb_kind);
	void enableScroll(bool e,int sb_kind);
	ScrollManager *_getScrollManager(int sb_kind);

	//�`��
	void repaint(bool erase){ InvalidateRect(hwnd,NULL,erase?TRUE:FALSE); }
	void repaint(int x,int y,int w,int h,bool erase){ RECT rc={x,y,x+w,y+h}; InvalidateRect(hwnd,&rc,erase?TRUE:FALSE); }
	void repaint(RECT *rc,bool erase){ InvalidateRect(hwnd,rc,erase?TRUE:FALSE); }
	void update(){ UpdateWindow(hwnd); }
	void scroll(int dx,int dy,bool erase)
	{
		ScrollWindowEx(hwnd,dx,dy,NULL,&rcClient,NULL,NULL,SW_INVALIDATE | (erase?SW_ERASE:0));
	}

	//�^�C�}�[
	void setTimer(int id,int interval){ SetTimer(hwnd,id,interval,NULL); }
	void killTimer(int id){ KillTimer(hwnd,id); }

	//�j��
	void setFocus(){ SetFocus(hwnd); }
	void destroy(){ DestroyWindow(hwnd); }
	void close(){ PostMessage(hwnd,WM_CLOSE,0,0); }

	//�c�[��
	void setCapture(){ SetCapture(hwnd); }
	void releaseCapture(){ ReleaseCapture(); }
	int messageBox(const wstring& text,const wstring& caption=L"",int type=0);
	int messageBoxF(const wstring& caption,int type,const wchar* text,...);
	void dragAcceptFiles(bool accept,bool notify=false);
	void setIcon(Icon *icon);
	LRESULT sendMessage(UINT msg,WPARAM wParam,LPARAM lParam){ return SendMessage(hwnd,msg,wParam,lParam); }
	LRESULT postMessage(UINT msg,WPARAM wParam,LPARAM lParam){ return SendMessage(hwnd,msg,wParam,lParam); }

	void scrollTop()
	{
		SendMessage(hwnd,WM_VSCROLL,MAKELONG(SB_TOP,0),NULL);
	}
	void scrollBottom()
	{
		SendMessage(hwnd,WM_VSCROLL,MAKELONG(SB_BOTTOM,0),NULL);
	}
public:
// ** ** ** ** ScrollManager ** ** ** ** //
	vector<ScrollManager*> scroll_list;
	void scrollRegist(ScrollManager *sm){ scroll_list.push_back(sm); }

// ** ** ** ** ���b�Z�[�W���� ** ** ** ** //
public:
	//
	static LRESULT CALLBACK WndProcWindow(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
	//
	virtual LRESULT onActivate(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onClose(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onCommand(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onCtlColor(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onDestroy(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onDropFiles(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onFocus(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onKey(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMenu(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMixer(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMouse(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMove(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onNotify(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onPaint(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onPaint(CDcGraphics *g);
	virtual LRESULT onScroll(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onSize(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onTaskTray(UINT msg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onTimer(UINT msg,WPARAM wParam,LPARAM lParam);

	virtual LRESULT onMessage(UINT msg,WPARAM wParam,LPARAM lParam,BOOL *processed);
	//
	LRESULT sendMenuClick(int id);
	LRESULT postMenuClick(int id);
	LRESULT sendSize(); //WM_SIZE�����Ăяo��
	LRESULT postSize(); //WM_SIZE�����Ăяo��
public:
	int testflag; //�f�o�b�O�p

};

#include "c_Icon.h"
#include "c_Graphics.h"
#include "c_ScrollManager.h"


