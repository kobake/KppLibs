
#pragma once

#include <windows.h>

class CSharedMemory{
public:
	//コンストラクタ・デストラクタ
	CSharedMemory(LPCWSTR szName, size_t nSize);
	virtual ~CSharedMemory();

	//取得
	void* GetPointer(){ return m_pSharedUse; }
	const void* GetPointer() const{ return m_pSharedUse; }
	DWORD GetSize() const{ return m_dwSize; }

	//属性
	bool IsCreated() const{ return m_bCreated; }

	//開発用
	HANDLE _GetHandle() const{ return m_hFileMapping; }
private:
	//生成パラメータ
	WCHAR	m_szName[64];
	DWORD	m_dwSize;

	//ハンドル等
	HANDLE	m_hFileMapping;
	void*	m_pShared;
	void*	m_pSharedUse; //実際に使う箇所

	//属性値
	bool	m_bCreated;
};
