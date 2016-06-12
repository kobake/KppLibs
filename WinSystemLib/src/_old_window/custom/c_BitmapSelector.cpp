#include <windows.h>
#include <cstdio>
#include <string>
using namespace std;

#include <BaseLib.h>
#include <BaseLibOld.h>
#include <GraphicsLib.h>
#include "../../_old_app/c_App.h"
#include "c_BitmapSelector.h"
#include "../f_window.h"


void BitmapSelector::drawData(CDcGraphics *g,int xoff,int yoff)
{
	if(bmp==NULL)return;
	int bw=bmp->GetSize().w;
	int bh=bmp->GetSize().h;
	int x=xoff*32;
	int y=yoff*32;
	int w=tmin<int>(rcClient.right,bw-x);
	int h=tmin<int>(rcClient.bottom,bh-y);
	//ビットマップ
	if(w>0 && h>0){
		g->DrawImage(CRect(0,0,w,h),*bmp,CRect(x,y,w,h));
	}
	//サイド塗りつぶし
	if(w<0)w=0;
	if(h<0)h=0;
	g->FillRect(CRect(0,h,rcClient.right-0,rcClient.bottom-h), CColor(128,128,128).win_color());
	g->FillRect(CRect(w,0,rcClient.right-w,h-0), CColor(128,128,128).win_color());
}




BitmapSelector::BitmapSelector(const wstring& caption,int x,int y,int w,int h,Window *_parent,int _option,int _id)
: ScrollDataWindow(WS_EX_TOOLWINDOW,WS_SYSMENU | WS_SIZEBOX | WS_HSCROLL | WS_VSCROLL,caption,x,y,w,h,_parent,_option,_id)
{
	bmp=NULL;
	sendSize();
}

BitmapSelector::~BitmapSelector()
{
}

void BitmapSelector::setBitmap(CDib *_bmp)
{
	bmp=_bmp;
	realizeBitmap();
}

void BitmapSelector::realizeBitmap()
{
	setCursorPos(0);
	Window::sendSize(); //スクロールバー更新
	repaint(false);
}


