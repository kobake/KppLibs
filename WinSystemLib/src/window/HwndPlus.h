#pragma once

#include <windows.h>

//Ç©Ç»ÇËHWNDÇ…ãﬂÇ¢ÉNÉâÉX
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
