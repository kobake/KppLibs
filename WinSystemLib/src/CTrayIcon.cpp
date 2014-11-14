#include "_required.h"
#include "CTrayIcon.h"
#include <StringLib.h>
#include <ShellAPI.h>
using namespace util;


CTrayIcon::CTrayIcon()
: m_hwnd(NULL)
, m_uiId(0)
, m_uiMsgId(0)
, m_hIcon(NULL)
, m_strTipText(L"")
{
}

CTrayIcon::CTrayIcon(
	HWND	hwnd,		//!< 通知を受け取るウィンドウハンドル
	UINT	nId,		//!< タスクトレイ内識別ID (アプリ内でユニークであれば良い)
	UINT	nMsgId,		//!< 通知メッセージID
	HICON	hIcon,		//!< アイコンイメージ
	LPCWSTR	szTipText	//!< チップテキスト
)
{
	Create(hwnd, nId, nMsgId, hIcon, szTipText);
}

CTrayIcon::~CTrayIcon()
{
	Destroy();
}

void CTrayIcon::Create(
	HWND	hwnd,		//!< 通知を受け取るウィンドウハンドル
	UINT	nId,		//!< タスクトレイ内識別ID (アプリ内でユニークであれば良い)
	UINT	nMsgId,		//!< 通知メッセージID
	HICON	hIcon,		//!< アイコンイメージ
	LPCWSTR	szTipText	//!< チップテキスト
)
{
	m_hwnd = hwnd;
	m_uiId = nId;
	m_uiMsgId = nMsgId;
	m_hIcon = hIcon;
	m_strTipText = szTipText;
	Create();
}

void CTrayIcon::Create()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uFlags=0;

	//識別
	nid.hWnd = m_hwnd;
	nid.uID = m_uiId;    //タスクトレイ内ID(アプリ内でユニークであれば良い)

	//コールバックメッセージ
	if(m_uiMsgId){
		nid.uFlags |= NIF_MESSAGE;
		nid.uCallbackMessage = m_uiMsgId;
	}
	
	//アイコン
	if(m_hIcon){
		nid.uFlags |= NIF_ICON;
		nid.hIcon = m_hIcon;
	}

	//チップテキスト
	if(m_strTipText.length()){
		nid.uFlags |= NIF_TIP;
		wcssafecpy(nid.szTip, m_strTipText.c_str(), countof(nid.szTip)); //チップテキスト
	}

	/*
		呼出

		登録できるまで繰り返す
		http://www31.ocn.ne.jp/~yoshio2/vcmemo17-1.html
	*/
	while(1){
		BOOL b = Shell_NotifyIcon(NIM_ADD, &nid);
		if(b){
			break;
		}
		else{
			// タイムアウト以外のエラーだったら諦める //
			if(::GetLastError() != ERROR_TIMEOUT){
				return;
			}

			// 何故か登録できていたら無問題 //
			if(Shell_NotifyIcon(NIM_MODIFY, &nid)){
				return;
			}

			// リトライまでの待ち時間 //
			::Sleep(1000);
		}
	}
}

void CTrayIcon::Destroy()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uFlags = 0;

	//識別
	nid.hWnd = m_hwnd;
	nid.uID = m_uiId;    //タスクトレイ内ID(アプリ内でユニークであれば良い)

	//呼出
	Shell_NotifyIcon(NIM_DELETE, &nid);
}
