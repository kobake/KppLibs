#include "common/_required.h"
#include "CCoolButton.h"
#include "GraphicsLib.h"
#include <Cast.h> //WinSystemLib

void CCoolButton::Create(const wchar_t* szCaption, const CRect& rc, HWND hwndParent)
{
	CWindow::Create(
		SWindowInfo(
			CWindowClass(c2lib::GetInstance(), L"BUTTON"),
			0,
			WS_VISIBLE | WS_CHILD | BS_FLAT | BS_CENTER | BS_VCENTER | BS_OWNERDRAW,
			szCaption,
			rc,
			hwndParent,
			NULL
		)
	);
}


LRESULT CCoolButton::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
	case WM_DRAWITEM:
		{
			DRAWITEMSTRUCT* p = (DRAWITEMSTRUCT*)lParam;
			if(p->itemState & ODS_SELECTED){
			}

		}
		break;
	default:
		return __super::OnMessage(msg, wParam, lParam);
	}
	return 0L;
}

bool CCoolButton::ProcessDrawItem(const DRAWITEMSTRUCT* lpdis) const
{
	if(lpdis->hwndItem == this->GetHWND()){
		CDcGraphics g(lpdis->hDC, RECT2Rect(lpdis->rcItem));
		//íÜêSç¿ïW
		int w = g.GetRect().Width();
		int h = g.GetRect().Height();
		int w2 = w/2 - 5;
		int h2 = h/2 - 5;
		CPoint pt(w/2, h/2);
		//îwåiêF
		COLORREF clrBack = ::GetSysColor(COLOR_BTNFACE);
		COLORREF clrMark = ::GetSysColor(COLOR_BTNTEXT);
		//ëIëéûÇÃïœâª
		if(lpdis->itemState & ODS_SELECTED){
//			pt.x += 1;
			clrBack = ::GetSysColor(COLOR_3DSHADOW);
			clrMark = ::GetSysColor(COLOR_BTNTEXT);
		}
		//îwåi
		g.FillRect(g.GetRect(), clrBack);
		//òg
		g.DrawRect(g.GetRect(), ::GetSysColor(COLOR_3DDKSHADOW));
		//Å~àÛ
		g.DrawLine(pt + CPoint(-w2,-h2), pt + CPoint(w2+1,h2+1), clrMark);
		g.DrawLine(pt + CPoint(-w2,h2), pt + CPoint(w2+1,-h2-1), clrMark);

		return true;
	}
	return false;
}
