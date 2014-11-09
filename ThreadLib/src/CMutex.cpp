#include "common/_required.h"
#include "CMutex.h"

CMutex::CMutex(LPCWSTR szMutexName, BOOL bInitialOwner)
{
	m_hMutex = CreateMutex(
		NULL,
		bInitialOwner, //�Ăяo���X���b�h�����̃~���[�e�b�N�X�����L���邩�ǂ���
		szMutexName
	);
	m_bCreated = (GetLastError()!=ERROR_ALREADY_EXISTS);
}

CMutex::~CMutex()
{
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
}

