#pragma once

#include <windows.h> //HANDLE

class CMutex{
public:
	//コンストラクタ・デストラクタ
	CMutex(LPCWSTR szMutexName, BOOL bInitialOwner = TRUE);
	virtual ~CMutex();

	//インターフェース
	bool IsCreated() const{ return m_bCreated; }

	//開発用
	HANDLE _GetMutexHandle() const{ return m_hMutex; }
private:
	HANDLE	m_hMutex;
	bool	m_bCreated; //新規に作成された場合はtrue
};
