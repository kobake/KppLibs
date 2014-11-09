#include "common/_required.h"
#include "CSharedMemory.h"
#include "CMutex.h"

CSharedMemory::CSharedMemory(LPCWSTR szName, size_t nSize)
{
	//生成パラメータ
	wcscpy_s(m_szName, _countof(m_szName), szName);
	m_dwSize = (DWORD)nSize;

	//マッピング
	m_hFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE | SEC_COMMIT,
		0,
		sizeof(DWORD) + m_dwSize,
		szName
	);

	//生成情報
	m_bCreated = (GetLastError()!=ERROR_ALREADY_EXISTS);

	//ビュー
	m_pShared = (void*)::MapViewOfFile(
		m_hFileMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0
	);

	//サイズ情報
	{
		CMutex cMutex(szName, FALSE); //マッピングと同じ名前でミューテックスを作成
		WaitForSingleObject(cMutex._GetMutexHandle(), INFINITE);
		if(m_bCreated){
			//マッピングを新規作成した場合は、そのサイズを書き込む
			memset(m_pShared, sizeof(DWORD) + m_dwSize, 0);	// まずゼロクリア //
			memcpy(m_pShared, &m_dwSize, sizeof(DWORD));	// 先頭4バイトにサイズをカキコ //
		}
		else{
			//既存のマッピングであった場合は、サイズを取得する
			memcpy(&m_dwSize, m_pShared, sizeof(DWORD));
		}
	}

	//実際に使う範囲
	m_pSharedUse = (BYTE*)m_pShared + sizeof(DWORD);

}

CSharedMemory::~CSharedMemory()
{
	UnmapViewOfFile(m_pShared);
	CloseHandle(m_hFileMapping);
}
