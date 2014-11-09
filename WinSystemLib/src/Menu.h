#pragma once

#include <windows.h>

class Menu{
public:
	Menu(HINSTANCE hInst,int id);
	~Menu();
	//
	void popup(HWND hwnd);
private:
	HMENU hMenu;
};
