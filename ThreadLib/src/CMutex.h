#pragma once

#include <windows.h> //HANDLE

class CMutex{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CMutex(LPCWSTR szMutexName, BOOL bInitialOwner = TRUE);
	virtual ~CMutex();

	//�C���^�[�t�F�[�X
	bool IsCreated() const{ return m_bCreated; }

	//�J���p
	HANDLE _GetMutexHandle() const{ return m_hMutex; }
private:
	HANDLE	m_hMutex;
	bool	m_bCreated; //�V�K�ɍ쐬���ꂽ�ꍇ��true
};
