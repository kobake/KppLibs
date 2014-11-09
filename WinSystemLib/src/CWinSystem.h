#pragma once

#include <windows.h>
#include <set>

typedef void (*VOID_FUNC)();

class CWinSystem{
public:
	//!ウィンドウクラス登録
	static ATOM RegisterWindowClass(
		const wchar_t* classname,
		WNDPROC wndproc,
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance
	);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                     メッセージループ                        //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

	//!通常のメッセージループ
	/*!
		通常のメッセージループ。WM_QUITを検出した時点で処理が戻る。

		@ret WM_QUIT時点でのMSG::wParam
	*/
	static int DoMessageLoop_Simple(VOID_FUNC PreIdle = NULL);

	//!モーダルダイアログ用メッセージループ
	/*!
		ダイアログが終了した時点で処理が戻る。

		@ret 未実装。今のところ常に0を返す。
	*/
	static int DoMessageLoop_Modal(HWND hwndDialog,HWND hwndOwner);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                   メッセージループ設定                      //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//アクセラレータ
	static void SetAccelerator(HACCEL hAccel){ m_hAccel = hAccel; }
	//メインウィンドウ
	static void SetMainWindow(HWND hwnd){ m_hwndMain = hwnd; }
	static HWND GetMainWindow(){ return m_hwndMain; }
	//ダイアログウィンドウ
	static void AddDialogWindow(HWND hwnd){ if(hwnd)m_setDialogs.insert(hwnd); }
	static void RemoveDialogWindow(HWND hwnd){ if(hwnd)m_setDialogs.erase(hwnd); }
//	static HWND GetDialogWindow(){ return m_hwndDialog; }
private:
	static HACCEL			m_hAccel;
	static HWND				m_hwndMain;
	static std::set<HWND>	m_setDialogs;
};


//警告封じラップ
inline LONG_PTR MySetWindowLongPtr(HWND hwnd, int nIndex, LONG_PTR dwNewLong)
{
	return ::SetWindowLongPtr(hwnd,nIndex,(LONG)dwNewLong);
}
inline LONG_PTR MyGetWindowLongPtr(HWND hwnd, int nIndex)
{
	return ::GetWindowLongPtr(hwnd,nIndex);
}


