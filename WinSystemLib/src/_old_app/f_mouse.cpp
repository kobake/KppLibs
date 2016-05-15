#include <windows.h>

void mouseMoveTo(int px,int py)
{
	RECT rc;
	GetClientRect(GetDesktopWindow(),&rc);
	px=MulDiv(px,65535,rc.right);  // = px / rc.right  * 65535;
	py=MulDiv(py,65535,rc.bottom); // = py / rc.bottom * 65535;
	mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE,px,py,0,0);
}

void mouseMove(int mx,int my)
{
	mouse_event(MOUSEEVENTF_MOVE,mx,my,0,0);
}

void mouseLClick()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

void mouseRClick()
{
	mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
}

void mouseLDown()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
}

void mouseLUp()
{
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

void mouseRDown()
{
	mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
}

void mouseRUp()
{
	mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
}

