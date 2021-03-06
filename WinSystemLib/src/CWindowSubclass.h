#pragma once

class CWindowSubclass{
public:
	CWindowSubclass();
	virtual ~CWindowSubclass();
	void SetSubclassWindow(HWND hwnd);
	void UnsetSubclassWindow();
	static LRESULT CALLBACK S_HookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//何か処理をしたらTRUEを返す
	virtual bool HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lresult) = 0;
private:
	HWND			m_hwnd;
};
