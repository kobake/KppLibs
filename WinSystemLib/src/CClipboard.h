#pragma once

#include <windows.h>

class CClipboard{
public:
	//コンストラクタ・デストラクタ
	CClipboard(HWND hwnd)						{ m_bIsValid = (::OpenClipboard(hwnd)!=FALSE); }
	~CClipboard()								{ Close(); }
	//インターフェース
	bool	IsValid() const						{ return m_bIsValid; }
	operator bool() const						{ return m_bIsValid; }
	//操作インターフェース
	void	Empty()								{ ::EmptyClipboard(); }
	void	SetData(UINT uFormat, HANDLE hMem)	{ ::SetClipboardData(uFormat, hMem); }
	void	Close()								{ if(m_bIsValid)::CloseClipboard(); m_bIsValid = false; }
	//取得インターフェース
	HANDLE	GetData(UINT uFormat)				{ return ::GetClipboardData(uFormat); }
	//インターフェース（そのうち削除）
	/*
	void setText(const wchar_t* text); //!<テキストを設定する
	wchar_t *getText();          //!<テキストを取得する。存在しない場合は NULL を返す。
	*/

	static UINT	S_GetCustomFormat(const wchar_t* szFormatName);
	static UINT S_CF_PREFERREDDROPEFFECT(); //※コピー元が設定するもの
	static UINT S_CF_PERFORMEDDROPEFFECT(); //※コピー先が設定するもの
	static UINT S_CF_PASTESUCCEEDED();
	static UINT S_CF_FILEDESCRIPTOR();
	static UINT S_CF_FILECONTENTS();
private:
	bool	m_bIsValid;
};
