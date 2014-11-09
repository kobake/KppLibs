#include "common/_required.h"
#include "CEvent.h"

CEvent::CEvent(
	LPCWSTR szName	//!< ���O�BNULL���w�肷��ƁA���O�����C�x���g���쐬�����B
)
{
	m_strEventName = szName?szName:L"";
	m_hEvent = ::CreateEvent(
		NULL,	//�Z�L�����e�B�L�q�q�BNULL���w�肷��ƁA�q�v���Z�X�͂��̃I�u�W�F�N�g�̃n���h�����p���ł��Ȃ��B
		TRUE,	//TRUE�c�蓮���Z�b�g FALSE�c�������Z�b�g (WaitForSingleObject�𔲂���Ǝ����Ŕ�V�O�i����Ԃɖ߂�)
		FALSE,	//FALSE�c������ԁ���V�O�i�����
		szName	//���O
	);
	m_bCreated = true;
}

CEvent::CEvent(HANDLE hEvent)	//�����̃C�x���g�����b�v����ꍇ
{
	m_hEvent = hEvent;
	m_bCreated = false;
}

CEvent::~CEvent()
{
	if(m_bCreated && m_hEvent!=NULL){
		CloseHandle(m_hEvent);
	}
}

//!�j��
void CEvent::Dispose()
{
	assert(m_hEvent!=NULL);
	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}
