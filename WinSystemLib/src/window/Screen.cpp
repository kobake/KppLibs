#include "_required.h"
#include "Screen.h"
#include <windows.h>
#include "../Cast.h"

Screen::Screen()
{
	update();
}

Screen::~Screen()
{
}

//!最新のスクリーン状態に更新
void Screen::update()
{
	RECT win_rc;
	GetClientRect(GetDesktopWindow(),&win_rc);
	rc=RECT2Rect(win_rc);
}
