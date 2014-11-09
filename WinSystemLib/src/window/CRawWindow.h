#pragma once

#include <windows.h> //HWND,DWORD,COLORREF
#include "../../../MathLib/MathLib.h" //CRect
#include <string> //wstring

//※初期化を自前で行う場合は、SetHWNDを呼び出す
/*
	CRawWindow自体はコピーが可能。（ただしその場合、ポリモーフィズムは失う）
	単純にHWNDの代わりとして振舞う。

	###そもそもCRawWindow自体にポリモーフィズムの機構(virtual)は不要かも。
*/
class CRawWindow{
public:
	//コンストラクタ・デストラクタ
	CRawWindow(HWND _hwnd=NULL) : m_hwnd(_hwnd) { }
	virtual ~CRawWindow(){}

	//ウィンドウハンドル
	void SetHWND(HWND _hwnd){ m_hwnd=_hwnd; }
	HWND GetHWND() const{ return m_hwnd; }

	//Windowsインターフェース
	virtual void Show();
	virtual void SetRect(const math::CRect& rc);
	virtual void SetVisible(bool b);
	virtual void Redraw();
	virtual void GetWindowRect(math::CRect* rc) const;
	virtual void GetWindowRect_OnParent(math::CRect* rc) const;
	virtual void GetClientRect(math::CRect* rc) const;
	virtual void Move(const math::CRect& rc);
	virtual void SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	virtual void KillTimer(UINT_PTR uIDEvent);
	virtual void MessageBox(LPCTSTR lpText, LPCTSTR lpCaption=NULL, UINT uType=MB_OK);
	virtual void SetFocus();
	virtual void SetText(const wchar_t* szText);
	virtual std::wstring GetText() const;
	virtual int GetTextLength() const;
	virtual void SetMenu(HMENU hMenu);

	//見た目
	virtual void SetFont(const wchar_t* szFace, int nPoint);
	virtual void SetFont(HFONT hFont);
	virtual void SetIcon(HICON hIcon);

	//Windowsインターフェースラップ
	math::CRect GetWindowRect() const{ math::CRect rc; GetWindowRect(&rc); return rc; }
	math::CRect GetWindowRect_OnParent() const{ math::CRect rc; GetWindowRect_OnParent(&rc); return rc; }
	math::CRect GetClientRect() const{ math::CRect rc; GetClientRect(&rc); return rc; }

	//特殊インターフェース
	void MoveToCenter();			//!<画面中心に移動
	void SendSizeMessage();			//!<WM_SIZEを明示的に発行
	void PostSizeMessage();			//!<WM_SIZEを明示的に発行
	void SetPos(int nX, int nY);	//!<サイズを保持して位置のみ変更
	void PostCommand(int nId);		//!<WM_COMMANDを擬似発行

	//メッセージ処理インターフェース
	LRESULT SendMessage  (UINT msg,WPARAM wparam,LPARAM lparam) const{ return ::SendMessage  (m_hwnd,msg,wparam,lparam); }
	BOOL    PostMessage  (UINT msg,WPARAM wparam,LPARAM lparam) const{ return ::PostMessage  (m_hwnd,msg,wparam,lparam); }
	LRESULT DefWindowProc(UINT msg,WPARAM wparam,LPARAM lparam){ return ::DefWindowProc(m_hwnd,msg,wparam,lparam); }

private:
	HWND m_hwnd;
};
