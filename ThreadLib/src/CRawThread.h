#pragma once

#include <windows.h>
#include "CEvent.h"
#include "CCriticalSection.h"

class CRawThread{
public:
	//�v���C�I���e�B
	enum EPriority{
		PRI_LOWEST	= THREAD_PRIORITY_LOWEST,
		PRI_LOW		= THREAD_PRIORITY_BELOW_NORMAL,
		PRI_NORMAL	= THREAD_PRIORITY_NORMAL,
		PRI_HIGH	= THREAD_PRIORITY_ABOVE_NORMAL,
		PRI_HIGHEST	= THREAD_PRIORITY_HIGHEST,
	};

public:
	//�������ƏI��
	CRawThread() : m_hThread(NULL) { }
	~CRawThread();

	//�����B��������������true��Ԃ��B�^�C���A�E�g�̏ꍇ��false�B�X���b�h�������ꍇ�͊����������Ă���Ƃ݂Ȃ��Atrue�B
	bool WaitForThreadEnd(int nTimeout = -1) const;

	//����
	void Suspend();
	void Resume();

	//����
	void SetPriority(EPriority ePriority);
	EPriority GetPriority() const;
protected:
	HANDLE		m_hThread;		//�X���b�h�n���h��
	//�N���e�B�J���Z�N�V����
	mutable CCriticalSection	m_cCritical;
};
