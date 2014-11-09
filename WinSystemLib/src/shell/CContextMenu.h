#pragma once

#include <shlobj.h>
#include "shared_shell_ptr.h"
#include <vector>
#include <string>
#include "../CWindowSubclass.h"
class CShellFolder;
class CItemIdList;

typedef shared_shell_ptr<IContextMenu> IContextMenuPtr;

/*!
	IContextMenuのラップ
*/
class CContextMenu : public CWindowSubclass{
public:
	//コンストラクタ・デストラクタ
	CContextMenu(
		const CShellFolder&	cShellFolder,	//!< フルパスフォルダ
		const CItemIdList&	cidlItem		//!< フォルダからの相対アイテムIDリスト
	);
	CContextMenu(	//実験
		const CShellFolder&				cShellFolder,
		const std::vector<CItemIdList>	vItems
	);
	virtual ~CContextMenu();

	//使用
	void AppendTo(
		HMENU	hMenu,
		int		nIdMin,
		int		nIdMax,
		bool	bCanRename = false,
		bool	bForDrive = false
	);
	void Invoke(int nId);
	std::wstring GetCommandString(int nId) const;

	//アクセス
	IContextMenu* _GetImp(){ return m_lpContextMenu.get(); }
	IContextMenu* operator-> () { return m_lpContextMenu.get(); }

	//拡張インターフェース
	IContextMenu2* I2(){ return static_cast<IContextMenu2*>(m_lpContextMenu.get()); }
	IContextMenu3* I3(){ return static_cast<IContextMenu3*>(m_lpContextMenu.get()); }

	//サブクラス化 //※SubclassWindowという名前は既にマクロとしてあるので、使えない。
//	void SetSubclassWindow(HWND hwnd);
//	void UnsetSubclassWindow();
	bool HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lresult); //何か処理をしたらTRUEを返す
//	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	IContextMenuPtr	m_lpContextMenu;
	int				m_nType;			// 0 or 1 or 2 or 3
	HWND			m_hwndSubclass;
	WNDPROC			m_wndprocSubclass;
	//
	int				m_nIdMin;	//AppendTo時のnIdMinを一時的に記録
};
