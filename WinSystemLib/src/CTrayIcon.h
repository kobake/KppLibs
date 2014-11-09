#pragma once

#include <windows.h>

class CTrayIcon{
public:
	// 初期化と終了
	CTrayIcon();
	CTrayIcon(
		HWND	hwnd,		//!< 通知を受け取るウィンドウハンドル
		UINT	nId,		//!< タスクトレイ内識別ID (アプリ内でユニークであれば良い)
		UINT	nMsgId,		//!< 通知メッセージID
		HICON	hIcon,		//!< アイコンイメージ
		LPCWSTR	szTipText	//!< チップテキスト
	);
	virtual ~CTrayIcon();
	void Create();
	void Create(
		HWND	hwnd,		//!< 通知を受け取るウィンドウハンドル
		UINT	nId,		//!< タスクトレイ内識別ID (アプリ内でユニークであれば良い)
		UINT	nMsgId,		//!< 通知メッセージID
		HICON	hIcon,		//!< アイコンイメージ
		LPCWSTR	szTipText	//!< チップテキスト
	);
	void Destroy();

private:
	HWND			m_hwnd;
	UINT			m_uiId;
	UINT			m_uiMsgId;
	HICON			m_hIcon;
	std::wstring	m_strTipText;
};

