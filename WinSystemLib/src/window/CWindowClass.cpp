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
		//������Ȃ�A��������R�s�[
		wcscpy_s(m_szMenuName, _countof(m_szMenuName), pMenuName);
	}
	else if(IS_INTRESOURCE(pMenuName)){
		//INTRESOURCE�Ȃ�A�|�C���^�l���R�s�[
		*((const wchar_t**)m_szMenuName) = pMenuName;
	}
}

bool CWindowClass::Regist(WNDPROC WndProc) const
{
	WNDCLASSEX wndclass;

	//�N���X�����ɑ��݂��Ă�����A�o�^���Ȃ�
	if(GetClassInfoEx(m_hInst,m_szClassName,&wndclass)){
		return false;
	}

	//�o�^
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
		throw myexception(tmp_swprintf(L"�E�B���h�E�N���X[%s]�̓o�^�Ɏ��s���܂���",m_szClassName));
	}
	return true;
}
