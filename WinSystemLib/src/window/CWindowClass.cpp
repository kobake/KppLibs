#include "_required.h"
#include "CWindowClass.h"
#include <StringLib.h>
#include <BaseLib.h>
using namespace util;


CWindowClass::CWindowClass()
{
	m_hInst = NULL;
	m_typeMenuName = 0;
	memset(m_szMenuName, 0, sizeof(m_szMenuName));
	m_hIcon = NULL;
}

CWindowClass::CWindowClass(HINSTANCE _hInst,const wchar_t* _class_name)
: m_hInst(_hInst)
{
	wcssafecpy(m_szClassName,_class_name,countof(m_szClassName));
	m_typeMenuName = 0;
	memset(m_szMenuName, 0, sizeof(m_szMenuName));
	m_hIcon = NULL;
}

CWindowClass::~CWindowClass()
{
}

void CWindowClass::SetMenuName(const wchar_t* pMenuName)
{
	if(pMenuName==NULL){
		//NULL
		m_typeMenuName = 0;
		m_szMenuName[0] = L'\0';
	}
	else if(!IS_INTRESOURCE(pMenuName)){
		//文字列なら、文字列をコピー
		wcscpy_s(m_szMenuName, _countof(m_szMenuName), pMenuName);
	}
	else if(IS_INTRESOURCE(pMenuName)){
		//INTRESOURCEなら、ポインタ値をコピー
		*((const wchar_t**)m_szMenuName) = pMenuName;
	}
}

bool CWindowClass::Regist(WNDPROC WndProc) const
{
	WNDCLASSEX wndclass;

	//クラスが既に存在していたら、登録しない
	if(GetClassInfoEx(m_hInst,m_szClassName,&wndclass)){
		return false;
	}

	//登録
	wndclass.cbClsExtra=0;
	wndclass.cbSize=sizeof(WNDCLASSEX);
	wndclass.cbWndExtra=0;
	wndclass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndclass.hIcon=m_hIcon;
	wndclass.hIconSm=NULL;
	wndclass.hInstance=m_hInst;
	wndclass.lpfnWndProc=WndProc;
	wndclass.lpszClassName=m_szClassName;
	wndclass.lpszMenuName=this->GetMenuName();
	wndclass.style=CS_HREDRAW | CS_VREDRAW;
	int ret=RegisterClassEx(&wndclass);
	if(ret==0){
		throw myexception(tmp_swprintf(L"ウィンドウクラス[%s]の登録に失敗しました",m_szClassName));
	}
	return true;
}
