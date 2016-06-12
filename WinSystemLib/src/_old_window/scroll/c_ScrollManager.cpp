#include <BaseLib.h>
#include <BaseLibOld.h>
#include "c_ScrollManager.h"
#include "../c_Window.h"
using namespace util;

ScrollManager::ScrollManager(Window *_wnd,int _sb_kind)
{
	init(_wnd,_sb_kind);

	if(_wnd!=NULL)_wnd->scrollRegist(this);
}
void ScrollManager::init(Window *_wnd,int _sb_kind)
{
	wnd=_wnd;
	sb_kind=_sb_kind;
	//
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_RANGE | SIF_POS | SIF_DISABLENOSCROLL;
	si.nMin=0;
	si.nMax=0;
	si.nPage=0;
	si.nPos=0;
	si.nTrackPos=0;
	if(wnd!=NULL)SetScrollInfo(wnd->getHWND(),sb_kind,&si,TRUE);
}
ScrollManager::~ScrollManager()
{
}
void ScrollManager::setRange(int range_min,int range_max)
{
	si.fMask=SIF_RANGE | SIF_POS | SIF_DISABLENOSCROLL;
	si.nMin=range_min;
	si.nMax=range_max;
	//nPosí≤êÆ
	if(range_min>range_max)iswap(&range_min,&range_max);
	if(si.nPos<range_min)si.nPos=range_min;
	else if(si.nPos>range_max)si.nPos=range_max;
	//
	if(wnd!=NULL)SetScrollInfo(wnd->getHWND(),sb_kind,&si,TRUE);
}
void ScrollManager::setPageSize(int page_size)
{
	si.fMask=SIF_PAGE | SIF_DISABLENOSCROLL;
	si.nPage=page_size;
	if(wnd!=NULL)SetScrollInfo(wnd->getHWND(),sb_kind,&si,TRUE);
}

LRESULT ScrollManager::onScroll(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(sb_kind==SB_HORZ && msg!=WM_HSCROLL)return 0L;
	if(sb_kind==SB_VERT && msg!=WM_VSCROLL)return 0L;
	if(sb_kind==SB_CTL && (HWND)lParam!=wnd->getHWND())return 0L;
	//
	si.fMask=SIF_POS | SIF_DISABLENOSCROLL;
	//
	if(msg==WM_HSCROLL){
		switch(LOWORD(wParam)){
		case SB_ENDSCROLL:
			break;
		// -- -- Å©Å© -- -- //
		case SB_LEFT:
			si.nPos=si.nMin;
			break;
		case SB_PAGELEFT:
			si.nPos-=tmax<int>(1,si.nPage/2);
		case SB_LINELEFT:
			si.nPos=tmax<int>(si.nMin,si.nPos-1);
			break;
		// -- -- Å®Å® -- -- //
		case SB_RIGHT:
			si.nPos=si.nMax;
			break;
		case SB_PAGERIGHT:
			si.nPos+=tmax<int>(1,si.nPage-1);
		case SB_LINERIGHT:
			si.nPos=tmin<int>(si.nMax,si.nPos+1);
			break;
		// -- -- ÅóÅó -- -- //
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			si.nPos=HIWORD(wParam);
			break;
		}
	}else if(msg==WM_VSCROLL){
		switch(LOWORD(wParam)){
		case SB_ENDSCROLL:
			break;
		// -- -- Å™Å™ -- -- //
		case SB_TOP:
			si.nPos=si.nMin;
			break;
		case SB_PAGEUP:
			si.nPos-=tmax<int>(1,si.nPage/2);
		case SB_LINEUP:
			si.nPos=tmax<int>(si.nMin,si.nPos-1);
			break;
		// -- -- Å´Å´ -- -- //
		case SB_BOTTOM:
			si.nPos=si.nMax;
			break;
		case SB_PAGEDOWN:
			si.nPos+=tmax<int>(1,si.nPage/2);
		case SB_LINEDOWN:
			si.nPos=tmin<int>(si.nMax,si.nPos+1);
			break;
		// -- -- ÅóÅó -- -- //
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			si.nPos=HIWORD(wParam);
			break;
		}
	}
	if(wnd!=NULL)SetScrollInfo(wnd->getHWND(),sb_kind,&si,TRUE);
	return 0L;
}

int ScrollManager::getPos()
{
	return si.nPos;
}
void ScrollManager::setPos(int pos)
{
	si.fMask=SIF_POS;
	si.nPos=adjust(pos,si.nMin,si.nMax);
	if(wnd!=NULL)SetScrollInfo(wnd->getHWND(),sb_kind,&si,TRUE);
}

void ScrollManager::enable(bool e)
{
	if(sb_kind==SB_CTL){
		EnableWindow(wnd->getHWND(),e);
	}else{
		if(!e){
			SCROLLINFO si;
			si.fMask=SIF_RANGE | SIF_DISABLENOSCROLL;
			si.nMin=si.nMax=0;
			if(wnd!=NULL)SetScrollInfo(wnd->getHWND(),sb_kind,&si,TRUE);
		}else{
			setRange(si.nMin,si.nMax);
		}
	}
}

