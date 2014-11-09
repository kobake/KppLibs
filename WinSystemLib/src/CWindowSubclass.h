#pragma once

class CWindowSubclass{
public:
	CWindowSubclass();
	virtual ~CWindowSubclass();
	void SetSubclassWindow(HWND hwnd);
	void UnsetSubclassWindow();
	static LRESULT CALLBACK S_HookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//âΩÇ©èàóùÇÇµÇΩÇÁTRUEÇï‘Ç∑
	virtual bool HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lresult) = 0;
private:
	HWND			m_hwnd;
};
