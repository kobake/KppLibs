#include <BaseLib.h>
#include "CContextMenu.h"
#include "CShellFolder.h"
#include <map>
#include "../Cast.h"
#include <vector>
using namespace std;

static map<HWND,CContextMenu*> g_mapContextMenus;

CContextMenu::CContextMenu(const CShellFolder& cShellFolder, const CItemIdList& cidlItem)
: m_hwndSubclass(NULL)
, m_wndprocSubclass(NULL)
, m_nIdMin(0)
{
	m_lpContextMenu = IContextMenuPtr((IContextMenu*)NULL);
	m_nType = 0;

	//まずはIContextMenu
	LPCITEMIDLIST a[] = {cidlItem._GetImp()};
	IContextMenu* lpContextMenu = cShellFolder.CreateContextMenu(a,1);
	if(lpContextMenu){
		m_lpContextMenu = IContextMenuPtr(lpContextMenu);
		m_nType = 1;
	}
	else{
		return; //失敗
	}
	
	//IContextMenu3を試す
	if(NOERROR == m_lpContextMenu->QueryInterface(IID_IContextMenu3, (void**)&lpContextMenu)){
		m_lpContextMenu = IContextMenuPtr(lpContextMenu);
		m_nType = 3;
		return;
	}

	//IContextMenu2を試す
	if(NOERROR == m_lpContextMenu->QueryInterface(IID_IContextMenu3, (void**)&lpContextMenu)){
		m_lpContextMenu = IContextMenuPtr(lpContextMenu);
		m_nType = 2;
	}
}

//#####実験
CContextMenu::CContextMenu(
	const CShellFolder&				cShellFolder,
	const std::vector<CItemIdList>	vItems			//###効率悪し
)
: m_hwndSubclass(NULL)
, m_wndprocSubclass(NULL)
, m_nIdMin(0)
{
	IContextMenu* lpContextMenu;

	m_lpContextMenu = IContextMenuPtr((IContextMenu*)NULL);
	m_nType = 0;

	//まずはIContextMenu
	{
		vector<const ITEMIDLIST*> a;
		for(int i=0;i<(int)vItems.size();i++){
			a.push_back(vItems[i]._GetImp());
		}
		lpContextMenu = cShellFolder.CreateContextMenu(a.size()?&a[0]:NULL, (int)a.size());
		if(lpContextMenu){
			m_lpContextMenu = IContextMenuPtr(lpContextMenu);
			m_nType = 1;
		}
		else{
			return; //失敗
		}
	}
	
	//IContextMenu3を試す
	if(NOERROR == m_lpContextMenu->QueryInterface(IID_IContextMenu3, (void**)&lpContextMenu)){
		m_lpContextMenu = IContextMenuPtr(lpContextMenu);
		m_nType = 3;
		return;
	}

	//IContextMenu2を試す
	if(NOERROR == m_lpContextMenu->QueryInterface(IID_IContextMenu3, (void**)&lpContextMenu)){
		m_lpContextMenu = IContextMenuPtr(lpContextMenu);
		m_nType = 2;
	}
}

CContextMenu::~CContextMenu()
{
}

//何か処理をしたらTRUEを返す
bool CContextMenu::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lresult)
{
	*lresult = 0;
	HRESULT hr = 0;

	switch (msg){ 
	case WM_MENUCHAR:	// only supported by IContextMenu3
		if(m_nType == 3)
		{
			*lresult = 0L;
			hr = I3()->HandleMenuMsg2 (msg, wParam, lParam, lresult);
			if(hr!=S_OK)return false; //結果がS_OKでなかった場合は、処理をしなかったものとみなす
			return true;
		}
		break;

	case WM_DRAWITEM:
	case WM_MEASUREITEM:
		if (wParam) 
			return false; // if wParam != 0 then the msg is not menu-related
  
	case WM_INITMENUPOPUP:
		if (m_nType==2)
			hr = I2()->HandleMenuMsg (msg, wParam, lParam);
		else // version 3
			hr = I3()->HandleMenuMsg (msg, wParam, lParam);
		if(hr!=S_OK)return false; //結果がS_OKでなかった場合は、処理をしなかったものとみなす
		*lresult = (msg == WM_INITMENUPOPUP ? 0 : TRUE); // inform caller that we handled WM_INITPOPUPMENU by ourself
		return true;
		break;
	}
	return false;
}

/*
void CContextMenu::SetSubclassWindow(HWND hwnd)
{
	if(m_nType>1){
		m_hwndSubclass = hwnd;
		m_wndprocSubclass = LongToPointer<WNDPROC>(SetWindowLong(hwnd,GWL_WNDPROC,PointerToLong(HookWndProc)));
		g_mapContextMenus[hwnd] = this;
	}
}
void CContextMenu::UnsetSubclassWindow()
{
	if(m_hwndSubclass){
		SetWindowLong(m_hwndSubclass, GWL_WNDPROC, PointerToLong(m_wndprocSubclass));
		g_mapContextMenus.erase(m_hwndSubclass);
		m_hwndSubclass = NULL;
		m_wndprocSubclass = NULL;
	}
}
*/

/*
LRESULT CALLBACK CContextMenu::HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(g_mapContextMenus.find(hwnd)!=g_mapContextMenus.end()){
		CContextMenu* context = g_mapContextMenus[hwnd];
		LRESULT lresult = 0L;
		BOOL bProcessed = context->DispatchMessage(hwnd, msg, wParam, lParam, &lresult);
		if(bProcessed){
			return lresult;
		}
	}
	switch (msg){ 
	case WM_DESTROY:
		{
			WNDPROC wndprocOld = (WNDPROC)GetProp ( hwnd, TEXT ("OldWndProc"));
			SetWindowLong(hwnd, GWL_WNDPROC, PointerToLong(wndprocOld));
			return ::CallWindowProc (wndprocOld, hwnd, msg, wParam, lParam);
		}
		break;

	default:
		break;
	}

	// call original WndProc of window to prevent undefined bevhaviour of window
	return ::CallWindowProc ((WNDPROC) GetProp ( hwnd, TEXT ("OldWndProc")), hwnd, msg, wParam, lParam);
}
*/

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           使用                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CContextMenu::AppendTo(
	HMENU	hMenu,
	int		nIdMin,
	int		nIdMax,
	bool	bCanRename,
	bool	bForDrive
)
{
	m_nIdMin = nIdMin;
	this->m_lpContextMenu->QueryContextMenu(
		hMenu,
		::GetMenuItemCount(hMenu),	//挿入を開始する場所
		nIdMin,
		nIdMax,
		CMF_NORMAL /*| CMF_EXPLORE*/ | (bCanRename?CMF_CANRENAME:0) | (bForDrive?CMF_NOVERBS:0)
	);
}

void CContextMenu::Invoke(int nId)
{
	CMINVOKECOMMANDINFO cmi = {0};
	cmi.cbSize = sizeof(cmi);
	cmi.lpVerb = MAKEINTRESOURCEA(nId - m_nIdMin);
	cmi.nShow = SW_SHOWNORMAL;
	this->m_lpContextMenu->InvokeCommand(&cmi);
}

std::wstring CContextMenu::GetCommandString(int nId) const
{
	wchar_t buf[128] = L"";
	HRESULT hr = m_lpContextMenu->GetCommandString(
		nId - m_nIdMin,
		GCS_VERBW,
		NULL,
		reinterpret_cast<char*>(buf), //引数の型は char だが、どうやら wchar_t が返ってくるらしい
		_countof(buf)
	);
	if(hr==NOERROR){
		return buf;
	}
	else{
		return L"";
	}
}
