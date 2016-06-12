#include <BaseLib.h>
#include "CShellFolder.h"
#include "CMalloc.h"
#include <comutil.h> //_bstr_t
#include "CEnumIdList.h"
#include <DebugLib.h>
#pragma comment(lib,"comsuppw.lib") //_bstr_t
using namespace std;


//デスクトップ
CDesktopFolder::CDesktopFolder()
: CShellFolder((const wchar_t*)1)
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  CShellFolder                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CShellFolder::CShellFolder(const wchar_t* szFolderPath)
{
	//無効
	if(szFolderPath==NULL){
		m_debug = L"null";
		m_cidl.Attach(NULL);
		m_lpShellFolder = IShellFolderSharedPtr((IShellFolder*)NULL);
	}
	//超スペシャルフォルダ (デスクトップ)
	else if(szFolderPath==(const wchar_t*)1){
		m_debug = L"desktop";
		//IShellFolder取得
		IShellFolder* psfTmp = NULL;
		SHGetDesktopFolder(&psfTmp);
		m_lpShellFolder = IShellFolderSharedPtr(psfTmp);

		//アイテムIDリストを構築
		LPITEMIDLIST pidl;
		::SHGetSpecialFolderLocation(
			NULL,
			CSIDL_DESKTOP,
			&pidl
		);
		m_cidl.Attach(pidl);
	}
	//スペシャルフォルダ
	else if(szFolderPath[0]==L':' && szFolderPath[1]==L':'){
		m_debug = szFolderPath;
		//マイコンピュータ
		if(wcscmp(szFolderPath,L"::MyComputer")==0){
			szFolderPath = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}";
			goto other_folder;
		}
		//コントロールパネル
		else if(wcscmp(szFolderPath,L"::ControlPanel")==0){
			szFolderPath = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{21EC2020-3AEA-1069-A2DD-08002B30309D}";
			goto other_folder;
		}
		//その他
		else{
			goto other_folder;
		}
	}
	//その他のフォルダ
	else{
		m_debug = szFolderPath;
other_folder:
		//デスクトップを用いる
		CDesktopFolder desktop;

		//アイテムIDリスト取得
		m_cidl = desktop.CreateItemIDList(szFolderPath);

		//###実験
		wstring test = m_cidl.GetPath();

		//IShellFolder取得
		m_lpShellFolder = IShellFolderSharedPtr(desktop.CreateShellFolder(m_cidl));
	}
	m_debug2 = m_cidl.GetPath();
}

void CShellFolder::Attach(IShellFolder* lpShellFolder)
{
//	if(lpShellFolder)lpShellFolder->AddRef(); //###要検証
	m_debug = L"attached";
	if(lpShellFolder){
		m_lpShellFolder = IShellFolderSharedPtr(lpShellFolder);
		m_cidl = this->CreateItemIDList(L""); //###要検証
	}
	else{
		m_lpShellFolder = IShellFolderSharedPtr((IShellFolder*)NULL);
		m_cidl.Clear(); //###要検証
	}
	m_debug2 = m_cidl.GetPath();
}

CShellFolder::~CShellFolder()
{
	//※shared_shell_ptrがReleaseしてくれるはず

	//if(m_lpShellFolder){
	//	m_lpShellFolder->Release();
	//}
}

//インターフェース
CItemIdList CShellFolder::CreateItemIDList(const wchar_t* szPath)
{
	LPITEMIDLIST pidl = NULL;
	_bstr_t strPath = szPath;
	HRESULT hr = m_lpShellFolder->ParseDisplayName(
		NULL,
		NULL,
		strPath,
		NULL,
		&pidl,
		NULL
	);
	if(hr!=NOERROR){
		ErrorLog(LEVEL_DEVELOPING_ERROR,L"◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆\r\n");
		ErrorLog(LEVEL_DEVELOPING_ERROR,L"Critical Error: ParseDisplayName failed.\r\n");
		ErrorLog(LEVEL_DEVELOPING_ERROR,L"◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆\r\n");
	}
	//クラスにアタッチ
	CItemIdList cidl;
	cidl.Attach(pidl);

	//###実験
	wstring test = cidl.GetPath(*this);

	return cidl;
}

IShellFolder* CShellFolder::CreateShellFolder(const CItemIdList& cidl)
{
	IShellFolder* lpShellFolder = NULL;
	HRESULT hRet = m_lpShellFolder->BindToObject(
		cidl._GetImp(),
		NULL,
		IID_IShellFolder,
		(LPVOID*)&lpShellFolder
	);
	if(hRet==NOERROR){
		return lpShellFolder;
	}
	else{
		return NULL;
	}
}

CEnumIdList CShellFolder::CreateEnumIDList() const
{
	IEnumIDList* pEnum;

	//列挙フラグ
	SHCONTF eFlags = 0;
	eFlags |= SHCONTF_FOLDERS;
	eFlags |= SHCONTF_NONFOLDERS;
	eFlags |= SHCONTF_INCLUDEHIDDEN;
	eFlags |= SHCONTF_INIT_ON_FIRST_NEXT;
//	eFlags |= SHCONTF_NETPRINTERSRCH;
	eFlags |= SHCONTF_SHAREABLE;
	eFlags |= SHCONTF_STORAGE;

	m_lpShellFolder->EnumObjects(NULL,eFlags,&pEnum);
	return CEnumIdList(pEnum);
}

#include <StringLib.h>

std::wstring CShellFolder::GetDisplayNameOfItem(const CItemIdList& cidl) const
{
	STRRET	ret;
	HRESULT hr = m_lpShellFolder->GetDisplayNameOf( cidl._GetImp(), SHGDN_NORMAL, &ret ); //###str.pOleStrを解放せんといかん

	//API失敗
	if(FAILED(hr))
		return L"<? GetDisplayNameOf failed ?>";

	//分類
	switch(ret.uType){
	case STRRET_WSTR:	return ret.pOleStr;
	case STRRET_OFFSET:	return (LPCTSTR)((char*)cidl._GetImp() + ret.uOffset);
	case STRRET_CSTR:	return cpp_mbstowcs(ret.cStr);
	}

	//分類失敗
	return L"<? GetDisplayNameOfItem failed ?>";
}

IContextMenu* CShellFolder::CreateContextMenu(LPCITEMIDLIST* apIDL, int nIDL) const
{
	IContextMenu* lpContextMenu;
	HRESULT hr = m_lpShellFolder->GetUIObjectOf(NULL, nIDL, apIDL, IID_IContextMenu, NULL, (LPVOID*)&lpContextMenu);
	if(FAILED(hr))
		return NULL;
	return lpContextMenu;
}

