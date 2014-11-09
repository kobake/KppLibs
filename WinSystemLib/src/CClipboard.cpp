#include "_required.h"
#include <windows.h>
#include "CClipboard.h"
#include <map>
#include <string>
#include <shlobj.h> //CFSTR_PREFERREDDROPEFFECT

UINT CClipboard::S_GetCustomFormat(const wchar_t* szFormatName)
{
	static std::map<std::wstring, UINT> table;
	UINT nFormat = table[szFormatName];
	if(nFormat == 0){
		nFormat = ::RegisterClipboardFormat(szFormatName);
		table[szFormatName] = nFormat;
	}
	return nFormat;
}

UINT CClipboard::S_CF_PREFERREDDROPEFFECT()
{
	static UINT ui = 0;
	if(ui == 0){
		ui = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	}
	return ui;
}

UINT CClipboard::S_CF_PERFORMEDDROPEFFECT()
{
	static UINT ui = 0;
	if(ui == 0){
		ui = RegisterClipboardFormat(CFSTR_PERFORMEDDROPEFFECT);
	}
	return ui;
}

UINT CClipboard::S_CF_PASTESUCCEEDED()
{
	static UINT ui = 0;
	if(ui == 0){
		ui = RegisterClipboardFormat(CFSTR_PASTESUCCEEDED);
	}
	return ui;
}

UINT CClipboard::S_CF_FILEDESCRIPTOR()
{
	static UINT ui = 0;
	if(ui == 0){
		ui = RegisterClipboardFormat(CFSTR_FILEDESCRIPTOR);
	}
	return ui;
}

UINT CClipboard::S_CF_FILECONTENTS()
{
	static UINT ui = 0;
	if(ui == 0){
		ui = RegisterClipboardFormat(CFSTR_FILECONTENTS);
	}
	return ui;
}

/*
void CClipboard::setText(const wchar_t* text)
{
	HGLOBAL hMem=GlobalAlloc(GHND,(wcslen(text)+1)*sizeof(wchar_t));
	wchar_t *p=(wchar_t*)GlobalLock(hMem);
	wcscpy(p,text);
	GlobalUnlock(hMem);
	if(OpenClipboard(NULL)){
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT,hMem);
		CloseClipboard();
	}else{
		GlobalFree(hMem);
	}
}

wchar_t *CClipboard::getText()
{
	if(OpenClipboard(NULL)){
		HGLOBAL hMem=GetClipboardData(CF_UNICODETEXT);
		if(hMem==NULL){
			CloseClipboard();
			return NULL;
		}
		wchar_t *p=(wchar_t*)GlobalLock(hMem);
		wchar_t *ret=wcsdup(p);
		GlobalUnlock(hMem);
		CloseClipboard();
		return ret;
	}else{
		return NULL;
	}
}
*/
