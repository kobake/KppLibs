#pragma once

#include "CEvent.h"

//�X���b�h�֐�
namespace c2lib{

typedef DWORD (WINAPI *Thread_EntryPoint)(LPVOID);

HANDLE Thread_Begin(
	Thread_EntryPoint	lpEntryPoint,			//!< �G���g���|�C���g
	LPVOID				lpParam,				//!< ���[�U��`�l
	const CEvent*		pcInitDoneEvent	= NULL	//!< �X���b�h�������������C�x���g (�������K�v�Ȃ�n��)
);

}//namespace c2lib
