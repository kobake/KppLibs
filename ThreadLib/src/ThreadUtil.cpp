#include "common/_required.h"
#include "ThreadUtil.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       �X���b�h�֐�                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//########### CRT �ɉe����^���邽�߁ACreateThread�͎g���ׂ��ł͂Ȃ��B
//########### ����� _beginthreadex ���g�����ƁB

HANDLE c2lib::Thread_Begin(
	Thread_EntryPoint	lpEntryPoint,
	LPVOID				lpParam,
	const CEvent*		pcInitDoneEvent
)
{
	//�X���b�h�쐬
	DWORD dwThreadId = 0;
	HANDLE hThread = CreateThread(
		NULL,			//�Z�L�����e�B�L�q�q�BNULL���w�肷��ƁA�擾�����n���h�����p���ł��Ȃ��B
		0,				//�X�^�b�N�T�C�Y�B0���w�肷��ƁA�Ăяo�����Ɠ������A����̃T�C�Y�����蓖�Ă���B
		lpEntryPoint,	//�G���g���[�|�C���g
		lpParam,		//�X���b�h�̈���
		0,				//�쐬�I�v�V����
		&dwThreadId		//[out] �X���b�hID
	);
	assert(hThread!=NULL);

	//�X���b�h������������������܂ő҂�
	if(pcInitDoneEvent){
		pcInitDoneEvent->WaitForSignal();
	}

	return hThread;
}
