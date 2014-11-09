#pragma once

#include <windows.h> //HINSTANCE

class CWindow;

class CWindowClass{
public:
	//コンストラクタ・デストラクタ
	CWindowClass();
	CWindowClass(HINSTANCE _hInst,const wchar_t* _class_name);
	~CWindowClass();

	//オプション設定
	void SetMenuName(const wchar_t* pMenuName);
	void SetIcon(HICON hIcon)					{ m_hIcon = hIcon; }

	//登録
	bool Regist(WNDPROC WndProc) const; //クラスを新しく登録したらtrue, 既に登録済みだったらfalse, それ以外のエラーでは例外を飛ばす

	//取得
	bool IsValid() const{ return GetName()!=NULL; }
	HINSTANCE GetModule() const{ return m_hInst; }
	const wchar_t* GetName() const{ return m_szClassName; }
	const wchar_t* GetMenuName() const
	{
		return
			(m_typeMenuName==0)?NULL:
			(m_typeMenuName==1)?m_szMenuName:
			(*(const wchar_t**)m_szMenuName);
	}

private:
	//必須
	HINSTANCE	m_hInst;
	wchar_t		m_szClassName[32];

	//オプション
	int			m_typeMenuName;		//0:NULL, 1:文字列, 2:INTRESOURCE
	wchar_t		m_szMenuName[32];	//空文字列の場合はNULL扱い
	HICON		m_hIcon;
};

