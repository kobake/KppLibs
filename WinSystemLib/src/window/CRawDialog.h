#pragma once

#include "CRawWindow.h"
#include "../CWinSystem.h"

class CRawDialog : public CRawWindow{
public:
	//ウィンドウハンドル
	void SetHWND(HWND hwnd)
	{
		__super::SetHWND(hwnd);
		CWinSystem::AddDialogWindow(hwnd);
	}

	//ダイアログインターフェース
	CRawWindow GetDlgItem(int nId) const
	{
		CRawWindow ret;
		ret.SetHWND(::GetDlgItem(this->GetHWND(), nId));
		return ret;
	}
	BOOL EndDialog(INT_PTR nResult)
	{
		return ::EndDialog(this->GetHWND(), nResult);
	}
};

/*
	CDialogを継承して新しいクラスを作り、OnMessageをオーバーライドして使う

	class CMyDialog : public CDialog{
	public:
		
	};
*/
class CDialog : public CRawDialog{
public:
	static INT_PTR CALLBACK S_DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	// 初期化と終了
	CDialog();
	~CDialog();
	int CreateModal(int dialogResourceId, HWND hwndParent);
	// イベント
	virtual BOOL OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

