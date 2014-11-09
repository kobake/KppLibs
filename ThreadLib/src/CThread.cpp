#include "common/_required.h"
#include "CThread.h"
#include <LogLib.h>
#include <objbase.h> // CoInitialize

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �������ƏI��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CThread::CThread()
{
	// �X���b�h�{�̂��Q�Ƃ��Ă���N���X�C���X�^���X�ւ̃|�C���^ //
	m_ppcThread = NULL;
}

CThread::~CThread()
{
	// �X���b�h�{�̂��Q�Ƃ��Ă���N���X�C���X�^���X�ւ̃|�C���^ //
	if(m_ppcThread){
		*m_ppcThread = NULL;
	}

	//�X���b�h���܂������Ă���ꍇ�́A�I������܂ő҂H
	if(m_hThread){
		KLOG(_FATAL, "[~CThread] Error: thread [%ls] is still running.", this->GetName());
#if 0
		while(1){
			bool bResult = WaitForThreadEnd(500);
			if(!bResult){
				break;
//				assert(0);
			}
		}
#endif
	}
	_Clear();
}

void CThread::_Clear()
{
	//�X���b�h�n���h�����������
	if(m_hThread){
		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_ppcThread = NULL;
	}
	//���̑��̃����o�ϐ����N���A����
	//�E���ɂȂ�
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        �����Ɣj��                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//����
void CThread::Create()
{
	CRITICAL_ENTER(m_cCritical);

	//���ɑ��݂��Ă�����G���[
	if(m_hThread){ assert(0); return; }

	//�X���b�h�쐬
	DWORD dwThreadId = 0;
	m_hThread = CreateThread(
		NULL,				//�Z�L�����e�B�L�q�q�BNULL���w�肷��ƁA�擾�����n���h�����p���ł��Ȃ��B
		0,					//�X�^�b�N�T�C�Y�B0���w�肷��ƁA�Ăяo�����Ɠ������A����̃T�C�Y�����蓖�Ă���B
		S_EntryPoint,		//�G���g���[�|�C���g
		this,				//�X���b�h�̈���
		CREATE_SUSPENDED,	//�쐬�I�v�V�����B�Ƃ肠���������J�n�͂��Ȃ�
		&dwThreadId			//[out] �X���b�hID
	);
	assert(m_hThread);

	//�X���b�h�J�n
	::ResumeThread(m_hThread);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

DWORD WINAPI CThread::S_EntryPoint(LPVOID lpParameter)
{
	CThread* pcThread=reinterpret_cast<CThread*>(lpParameter);

	// �X���b�h�{�̂��Q�Ƃ��Ă���N���X�C���X�^���X�ւ̃|�C���^ //
	pcThread->m_ppcThread = &pcThread;

	//����������
	HRESULT ret=CoInitialize(NULL);
	assert(ret==S_OK);
	if(pcThread){
		pcThread->OnThreadBegin();
	}

	//����
	DWORD dwExitCode = 0;
	if(pcThread){
		dwExitCode = pcThread->Run();
	}

	//�����I��
	if(pcThread){
		pcThread->OnThreadEnd();
		pcThread->_Clear();
	}
	CoUninitialize();

	return dwExitCode;
}

