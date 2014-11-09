#pragma once

#include "CRawDialog.h"
#include "CWindowClass.h"
#include <vector>
#include "util/vector_ex.h"

/*
┌─────────────┐
│ CDialogPanelクラス詳細説明     │
└─────────────┘
 (削除タイミング)
 ・ウィンドウが存在する状態で、~CDialogPanelが呼ばれると、
 　擬似的なWM_DESTROYが呼ばれ、ウィンドウは破棄されます。
 ・CDialogPanelが存在する状態で、ウィンドウが破棄されると、WM_DESTROYを受け取り、
   CDialogPanel内のhwndメンバがNULLになりますが、CDialogPanelインスタンスは存在し続けます。
   その後、~CDialogPanelが呼ばれたときは、既にウィンドウは存在しないので、
   WM_DESTROYは呼ばれず、破棄処理も行われません。
*/


class CDialogPanel;


//ウィンドウプロシージャの関数ポインタ
//trueを返した場合は、それ以降のウィンドウプロシージャを呼ばせない
typedef bool (*DLGPROC_EX)(
	CDialogPanel*	pcWnd,
	UINT			msg,
	WPARAM			wParam,
	LPARAM			lParam
);

//※初期化を自前で行う場合は、SetHWNDを呼び出す
//  ####ただしサブクラス化は未考慮
class CDialogPanel : public CRawDialog{ //######ここをCRawDialogから継承させるのが理想かも。
public:
	//コンストラクタ・デストラクタ
	CDialogPanel();
	virtual ~CDialogPanel();

	//生成と破棄
	void Create(int nResourceId, HWND hwndParent);
	void Destroy();
	void Attach(HWND hwnd);

	//開発用
	virtual BOOL OnMessage(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if(m_dlgprocSubclass){
			return (BOOL)::CallWindowProc((WNDPROC)m_dlgprocSubclass, this->GetHWND(), msg, wparam, lparam);
		}
		else{
			return FALSE; //デフォルトの処理に任せる
		}
	}

	//カスタマイズ
	void AddProcedure(DLGPROC_EX wndproc){ m_vHookProcs.push_back_unique(wndproc); }
	void RemoveProcedure(DLGPROC_EX wndproc){ m_vHookProcs.erase_value(wndproc); }

	//自家製インターフェース
	void SetExLong(long n);
	long GetExLong() const;


private:
	friend INT_PTR CALLBACK CDialog_DialogProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	INT_PTR _CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam);
private:
	/*
		(新しいウィンドウクラスを作った場合)
			ウィンドウプロシージャ: WndProcWindow
			m_dlgprocSubclass:              NULL
		(サブクラス化した場合)
			ウィンドウプロシージャ: WndProcWindow
			m_dlgprocSubclass:              元のウィンドウクラスのウィンドウプロシージャ

		◆ 呼び出しフロー ◆
		(システム)
		↓
		WndProcWindow (WNDPROC)
		│
		├→ m_hookprocs (DLGPROC_EX)
		│
		└→ OnMessage (virtual)
		　　　│
		  　　└→ m_dlgprocSubclass (WNDPROC)
		↓
		(システム)
	*/
	vector_ex<DLGPROC_EX>	m_vHookProcs;
	DLGPROC					m_dlgprocSubclass;

	//自由データ
	uchar freedata[32];
};





