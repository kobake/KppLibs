#include "common/_required.h"
#include "CSharedMemory.h"
#include "CMutex.h"

CSharedMemory::CSharedMemory(LPCWSTR szName, size_t nSize)
{
	//�����p�����[�^
	wcscpy_s(m_szName, _countof(m_szName), szName);
	m_dwSize = (DWORD)nSize;

	//�}�b�s���O
	m_hFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE | SEC_COMMIT,
		0,
		sizeof(DWORD) + m_dwSize,
		szName
	);

	//�������
	m_bCreated = (GetLastError()!=ERROR_ALREADY_EXISTS);

	//�r���[
	m_pShared = (void*)::MapViewOfFile(
		m_hFileMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0
	);

	//�T�C�Y���
	{
		CMutex cMutex(szName, FALSE); //�}�b�s���O�Ɠ������O�Ń~���[�e�b�N�X���쐬
		WaitForSingleObject(cMutex._GetMutexHandle(), INFINITE);
		if(m_bCreated){
			//�}�b�s���O��V�K�쐬�����ꍇ�́A���̃T�C�Y����������
			memset(m_pShared, sizeof(DWORD) + m_dwSize, 0);	// �܂��[���N���A //
			memcpy(m_pShared, &m_dwSize, sizeof(DWORD));	// �擪4�o�C�g�ɃT�C�Y���J�L�R //
		}
		else{
			//�����̃}�b�s���O�ł������ꍇ�́A�T�C�Y���擾����
			memcpy(&m_dwSize, m_pShared, sizeof(DWORD));
		}
	}

	//���ۂɎg���͈�
	m_pSharedUse = (BYTE*)m_pShared + sizeof(DWORD);

}

CSharedMemory::~CSharedMemory()
{
	UnmapViewOfFile(m_pShared);
	CloseHandle(m_hFileMapping);
}
