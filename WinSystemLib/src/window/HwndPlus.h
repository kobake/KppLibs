#pragma once

#include <windows.h>

//���Ȃ�HWND�ɋ߂��N���X
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
