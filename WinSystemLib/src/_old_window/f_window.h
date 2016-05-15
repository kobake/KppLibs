#pragma once


HWND CreateAWindow(HINSTANCE hInst,HINSTANCE hPrevInst,const wchar* classname,WNDPROC wndproc,HBRUSH hBrush,HICON hIcon,const wchar* szMenu,DWORD ex_style,DWORD style,const wchar* caption,int x,int y,int w,int h,HWND parent,HMENU hMenu);

wchar *tmp_GetWindowText(HWND hwnd);
int MessageBoxF(HWND hwnd,LPCTSTR lpCaption,UINT uType,LPCTSTR lpText,...);
BOOL vSetWindowTextF(HWND hwnd,LPCTSTR lpString,va_list mark);
BOOL SetWindowTextF(HWND hwnd,LPCTSTR lpString,...);
void SetFont(HWND hwnd,HFONT hFont);
void EnableMenuItemID(HMENU hMenu,int id,bool enable);
void CheckMenuItemID(HMENU hMenu,int id,bool checked);

