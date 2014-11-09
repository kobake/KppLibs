#include "common/_required.h"
#include "CBitmap.h"

CBitmap::CBitmap(HINSTANCE hInst,int id)
{
	m_hbmp=LoadBitmap(hInst,MAKEINTRESOURCE(id));
	m_hold=true;
	_calcSize();
}

CBitmap::CBitmap(HBITMAP _hBitmap,bool _hold)
{
	m_hbmp=_hBitmap;
	m_hold=_hold;
	_calcSize();
}

CBitmap::CBitmap()
{
	m_hbmp=NULL;
	m_hold=false;
	_calcSize();
}

CBitmap::~CBitmap()
{
	if(m_hold){
		DeleteObject(m_hbmp);
	}
}

//! ÉTÉCÉYÇéÊìæÇµÇƒï€éù
void CBitmap::_calcSize()
{
	BITMAP bmp_info;
	GetObject(m_hbmp, sizeof(BITMAP), &bmp_info);
	m_size.w = bmp_info.bmWidth;
	m_size.h = bmp_info.bmHeight;
}
