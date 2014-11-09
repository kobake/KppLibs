#pragma once

#include <window/CWindow.h> //WinSystemLib
#include <MathLib.h> //CRect
#include <windows.h> //HWND

class CCoolButton : public CWindow{
public:
	void Create(const wchar_t* szCaption);
	void ff(const CRect& rc);
	void ff(HWND hwndParent);
	void Create(const wchar_t* szCaption, const CRect& rc, HWND hwndParent);
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	bool ProcessDrawItem(const DRAWITEMSTRUCT* lpdis) const;
};
