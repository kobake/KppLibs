
#pragma once

#include <windows.h>

class CSharedMemory{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CSharedMemory(LPCWSTR szName, size_t nSize);
	virtual ~CSharedMemory();

	//�擾
	void* GetPointer(){ return m_pSharedUse; }
	const void* GetPointer() const{ return m_pSharedUse; }
	DWORD GetSize() const{ return m_dwSize; }

	//����
	bool IsCreated() const{ return m_bCreated; }

	//�J���p
	HANDLE _GetHandle() const{ return m_hFileMapping; }
private:
	//�����p�����[�^
	WCHAR	m_szName[64];
	DWORD	m_dwSize;

	//�n���h����
	HANDLE	m_hFileMapping;
	void*	m_pShared;
	void*	m_pSharedUse; //���ۂɎg���ӏ�

	//�����l
	bool	m_bCreated;
};
