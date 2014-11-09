#include "common/_required.h"
#include "CMutex.h"

CMutex::CMutex(LPCWSTR szMutexName, BOOL bInitialOwner)
{
	m_hMutex = CreateMutex(
		NULL,
		bInitialOwner, //呼び出しスレッドがこのミューテックスを所有するかどうか
		szMutexName
	);
	m_bCreated = (GetLastError()!=ERROR_ALREADY_EXISTS);
}

CMutex::~CMutex()
{
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
}

