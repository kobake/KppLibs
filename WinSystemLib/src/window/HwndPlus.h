#pragma once

#include <windows.h>

//かなりHWNDに近いクラス
class HwndPlus{
public:
	HwndPlus(HWND _hwnd);
	void writeWindowData(const void* data,size_t size);
	void readWindowData(void* data,size_t size);
	void setWindowPointer(const void* p);
	void* getWindowPointer();
private:
	HWND hwnd;
};
