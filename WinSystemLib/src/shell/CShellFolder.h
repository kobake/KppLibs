#pragma once

#include <shlobj.h> //IShellFolder
#include <string>   //std::wstring
#include "CItemIdList.h"
#include "shared_shell_ptr.h"
class CEnumIdList;

typedef shared_shell_ptr<IShellFolder> IShellFolderSharedPtr;


/*!
	IShellFolderのラッパー
	
	※コピー可能。

	使用例)
		CShellFolder f(L"::MyComputer");
		CShellFolder f(L"D:");
		CShellFolder f(L"D:\\_tmp");
*/
class CShellFolder{
public:
	//コンストラクタ・デストラクタ
	CShellFolder(const wchar_t* szFolderPath = NULL);
	virtual ~CShellFolder();
	void Attach(IShellFolder* lpShellFolder);
	bool IsValid() const{ return m_lpShellFolder!=NULL; }

	//インターフェース
	CItemIdList		CreateItemIDList(const wchar_t* szPath);
	IShellFolder*	CreateShellFolder(const CItemIdList& pidl);
	std::wstring	GetDisplayNameOfItem(const CItemIdList& cidl) const;
	IContextMenu*	CreateContextMenu(LPCITEMIDLIST* apIDL, int nIDL) const;

	// 列挙 //
	CEnumIdList		CreateEnumIDList() const;

	//開発用
	IShellFolder* _GetIShellFolder(){ return m_lpShellFolder.get(); }
	const CItemIdList& _GetItemIdList() const{ return m_cidl; }

	//演算子
	IShellFolder* operator -> () { return m_lpShellFolder.get(); }

private:
	std::wstring			m_debug;
	std::wstring			m_debug2;
	IShellFolderSharedPtr	m_lpShellFolder;
	CItemIdList				m_cidl;				//!<アイテムIDリスト。デスクトップの場合はNULL。###今んところ使われない。
};

//!IShellFolder(SHGetDesktopFolder)のラップ
class CDesktopFolder : public CShellFolder{
public:
	//コンストラクタ・デストラクタ
	CDesktopFolder();
};
