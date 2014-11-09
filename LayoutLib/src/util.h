#pragma once

#include <windows.h>

class CLockWindowUpdate{
public:
	CLockWindowUpdate(HWND hwnd)
	: m_hwnd(hwnd)
	{
		::LockWindowUpdate(m_hwnd);
	}
	~CLockWindowUpdate()
	{
		::LockWindowUpdate(NULL);
		::UpdateWindow(m_hwnd);
	}
private:
	HWND m_hwnd;
};
