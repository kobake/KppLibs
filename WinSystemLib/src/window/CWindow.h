#pragma once

#include "CRawWindow.h"
#include "CWindowClass.h"
#include <vector>
#include <BaseLib.h>


/*
┌─────────────┐
│ CWindowクラス詳細説明     │
└─────────────┘
 (削除タイミング)
 ・ウィンドウが存在する状態で、~CWindowが呼ばれると、
 　擬似的なWM_DESTROYが呼ばれ、ウィンドウは破棄されます。
 ・CWindowが存在する状態で、ウィンドウが破棄されると、WM_DESTROYを受け取り、
   CWindow内のhwndメンバがNULLになりますが、CWindowインスタンスは存在し続けます。
   その後、~CWindowが呼ばれたときは、既にウィンドウは存在しないので、
   WM_DESTROYは呼ばれず、破棄処理も行われません。
*/

class CWindowClass;
class CWindow;

struct SWindowInfo{
	//必須項目
	CWindowClass wclass;
	DWORD ex_style;
	DWORD style;
	const wchar_t* caption;
	const math::CRect rc;
	HWND hwndParent;

	//オプション
	HMENU hMenu;

	SWindowInfo(
		const CWindowClass&	_wclass,
		DWORD				_ex_style,
		DWORD				_style,
		const wchar_t*		_caption,
		const math::CRect&		_rc,
		HWND				_hwndParent,
		HMENU				_hMenu = NULL
	)
	: wclass(_wclass), ex_style(_ex_style), style(_style), caption(_caption), rc(_rc), hwndParent(_hwndParent), hMenu(_hMenu)
	{
	}
	SWindowInfo()
	: wclass()
	{
	}
	bool valid() const{ return wclass.IsValid(); }
};

//ウィンドウプロシージャの関数ポインタ
//trueを返した場合は、それ以降のウィンドウプロシージャを呼ばせない
typedef bool (*WNDPROC_EX)(
	CWindow*	pcWnd,
	UINT		msg,
	WPARAM		wParam,
	LPARAM		lParam,
	LRESULT*	pnResult
);

//※初期化を自前で行う場合は、SetHWNDを呼び出す
//  ####ただしサブクラス化は未考慮
class CWindow : public CRawWindow{
public:
	//コンストラクタ・デストラクタ
	CWindow();
	virtual ~CWindow();

	//生成と破棄
	void Create(const SWindowInfo& info);
	void Destroy();
	void Attach(HWND hwnd);

	//開発用
	virtual LRESULT OnMessage(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if(m_wndprocSubclass){
			return ::CallWindowProc(m_wndprocSubclass, this->GetHWND(), msg, wparam, lparam);
		}
		else{
			return ::DefWindowProc(this->GetHWND(), msg, wparam, lparam);
		}
	}

	//カスタマイズ
	void AddProcedure(WNDPROC_EX wndproc){ m_vHookProcs.push_back_unique(wndproc); }
	void RemoveProcedure(WNDPROC_EX wndproc){ m_vHookProcs.erase_value(wndproc); }

	//Windowsインターフェース (###このあたりは別クラスに継承して機能分離したいところ)
	virtual void SetTransparentColor(COLORREF _transparent_color);
	virtual void SetAlpha(uchar _alpha);
	virtual uchar GetAlpha() const{ return m_btAlpha; }
	virtual COLORREF GetTransparentColor() const{ return m_clrTransparent; }
	
	//自家製インターフェース
	void SetExLong(long n);
	long GetExLong() const;

private:
	friend LRESULT CALLBACK WndProcWindow(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	LRESULT _CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam);
private:
	/*
		(新しいウィンドウクラスを作った場合)
			ウィンドウプロシージャ: WndProcWindow
			m_wndprocSubclass:              NULL
		(サブクラス化した場合)
			ウィンドウプロシージャ: WndProcWindow
			m_wndprocSubclass:              元のウィンドウクラスのウィンドウプロシージャ

		◆ 呼び出しフロー ◆
		(システム)
		↓
		WndProcWindow (WNDPROC)
		│
		├→ m_hookprocs (WNDPROC_EX)
		│
		└→ OnMessage (virtual)
		　　　│
		  　　└→ m_wndprocSubclass (WNDPROC)
		↓
		(システム)
	*/
	vector_ex<WNDPROC_EX>	m_vHookProcs;
	WNDPROC					m_wndprocSubclass;

	//Windows設定保持
	COLORREF m_clrTransparent;
	uchar m_btAlpha;

	//自由データ
	uchar m_freedata[32];
};
