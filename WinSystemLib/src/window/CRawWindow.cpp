#include "_required.h"
#include "CRawWindow.h"
#include "../Cast.h"
#include "Screen.h"
#include <vector>
#include <string>
#include <DebugLib.h>
#include <Windows.h>
#include <CommDlg.h>
using namespace std;
using namespace math;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CRawWindow::Show()
{
	ShowWindow(m_hwnd,SW_SHOWNORMAL);
}

void CRawWindow::SetRect(const math::CRect& rc)
{
	SetWindowPos(m_hwnd,NULL,rc.Left(),rc.Top(),rc.Width(),rc.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
}

void CRawWindow::SetVisible(bool b)
{
	if(b){
		ShowWindow(m_hwnd,SW_SHOW);
	}else{
		ShowWindow(m_hwnd,SW_HIDE);
	}
}

void CRawWindow::Redraw()
{
	InvalidateRect(m_hwnd,NULL,FALSE);
}

void CRawWindow::GetWindowRect(math::CRect* rc) const
{
	RECT w;
	::GetWindowRect(m_hwnd,&w);
	*rc=RECT2Rect(w);
}

void CRawWindow::GetWindowRect_OnParent(math::CRect* rc) const
{
	RECT w;
	::GetWindowRect(m_hwnd,&w);
	HWND hwndParent = ::GetParent(m_hwnd);
	if(hwndParent){
		::ScreenToClient(hwndParent, (POINT*)&w.left); //left,top
		::ScreenToClient(hwndParent, (POINT*)&w.right); //right,bottom
	}
	*rc=RECT2Rect(w);
}

void CRawWindow::GetClientRect(math::CRect* rc) const
{
	RECT w;
	::GetClientRect(m_hwnd,&w);
	*rc=RECT2Rect(w);
}

void CRawWindow::Move(const math::CRect& rc)
{
	::MoveWindow(m_hwnd,rc.Left(),rc.Top(),rc.Width(),rc.Height(),TRUE);
}

void CRawWindow::SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
{
	::SetTimer(m_hwnd,nIDEvent,uElapse,lpTimerFunc);
}

void CRawWindow::KillTimer(UINT_PTR uIDEvent)
{
	::KillTimer(m_hwnd,uIDEvent);
}

void CRawWindow::MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	//lpCaptionがデフォルト値であった場合
	if(lpCaption==NULL){
		static TCHAR title[_MAX_PATH]=_T("");
		if(title[0]==_T('\0')){
			TCHAR path[_MAX_PATH];
			::GetModuleFileName(NULL,path,_countof(path));
			::GetFileTitle(path,title,_countof(title));
		}
		lpCaption=title;
	}

	::MessageBox(m_hwnd,lpText,lpCaption,uType);
}

void CRawWindow::SetFocus()
{
	::SetFocus(m_hwnd);
}

void CRawWindow::SetText(const wchar_t* szText)
{
	::SetWindowText(m_hwnd, szText);
}

std::wstring CRawWindow::GetText() const
{
	int nLen = ::GetWindowTextLength(m_hwnd);
	vector<wchar_t> buf(nLen+1);
	::GetWindowText(m_hwnd, &buf[0], (int)buf.size());
	return &buf[0];
}

int CRawWindow::GetTextLength() const
{
	return ::GetWindowTextLength(m_hwnd);
}

void CRawWindow::SetFont(const wchar_t* szFace, int nPoint)
{
	assert(0); //####未実装
}

void CRawWindow::SetFont(HFONT hFont)
{
	::SendMessage(m_hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
}

void CRawWindow::SetIcon(HICON hIcon)
{
	::SendMessage(m_hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	::SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

void CRawWindow::SetMenu(HMENU hMenu)
{
	::SetMenu(m_hwnd, hMenu);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                   特殊インターフェース                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void CRawWindow::MoveToCenter()
{
	math::CRect screen=Screen().getRect();
	math::CRect rc; this->GetWindowRect(&rc);

	rc.Assign(screen.Width()/2,screen.Height()/2,rc.Width(),rc.Height(),X_CENTER | Y_CENTER);

	Move(rc);
}

//!WM_SIZEを明示的に発行
void CRawWindow::SendSizeMessage()
{
	RECT rc;
	::GetClientRect(m_hwnd,&rc);
	::SendMessage(
		m_hwnd,
		WM_SIZE,
		0, //### wparamは未実装
		MAKELPARAM(rc.right,rc.bottom)
	);
}

//!WM_SIZEを明示的に発行
void CRawWindow::PostSizeMessage()
{
	RECT rc;
	::GetClientRect(m_hwnd,&rc);
	::PostMessage(
		m_hwnd,
		WM_SIZE,
		0, //### wparamは未実装
		MAKELPARAM(rc.right,rc.bottom)
	);
}

//!サイズを保持して位置のみ変更
void CRawWindow::SetPos(int nX, int nY)
{
	RECT rc;
	::GetWindowRect(m_hwnd,&rc);
	int w = rc.right-rc.right;
	int h = rc.bottom-rc.top;
	::MoveWindow(m_hwnd,nX,nY,w,h,TRUE);
}

//! WM_COMMANDを擬似発行
void CRawWindow::PostCommand(int nId)
{
	::PostMessage(m_hwnd, WM_COMMAND, MAKEWPARAM(nId, NULL), MAKELPARAM(0,0));
}
