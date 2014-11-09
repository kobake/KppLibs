#pragma once

#include "CEvent.h"
#include "CCriticalSection.h"
#include <windows.h>
#include "design/CNamable.h"
#include "CRawThread.h"
using namespace c2lib;

/*!
	�X���b�h�N���X

	�X���b�h����������ꍇ�A���̃N���X���p�����Arun���`����B

	�y�����z
	new���������ł̓X���b�h�͍쐬����Ȃ��B
	Create()���Ă񂾎��_�ŃX���b�h���쐬����A���̃X���b�h�͑��J�n�����B

	�y�j���z
	Destroy()���ĂԂƃX���b�h�͒��f����j�������B
	�f�X�g���N�^���ɂ�Destroy()�͌Ă΂��B(�X���b�h���s���Ƀf�X�g���N�^���Ă΂ꂽ�ꍇ�A�X���b�h�͂��̎��_�Œ��f����)

	�X���b�h���삪��������ƁA��Ԃ� STATE_DONE �ƂȂ�B(����delete�͂���Ȃ�)
*/
/*
	�V�d�l�̍l��
	STATE_NONE
	��
	��Create()
	��
	STATE_RUNNING


	STATE_RUNNING
	��
	��Destroy() �܂��̓X���b�h����
	��
	STATE_NONE

	�y�C�x���g�z
	OnThreadBegin
	OnThreadEnd
*/
class CThread : public CRawThread, public CNameHolder{
public:
	// -- -- �^ -- -- //
	//���
	enum EState{
		STATE_NONE,		//��������Ă��Ȃ�
		STATE_RUNNING,		//���쒆
		STATE_SUSPENDED,	//�ꎞ��~��
		STATE_DONE,			//����I����
	};
protected:
	//static
	static DWORD WINAPI S_EntryPoint(LPVOID lpParameter);

public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CThread();
	virtual ~CThread();
	void _Clear();

	//����
	void Create();					// �g���l���Ăт܂��傤 //

	//��ԃ��b�v
	bool IsRunning() const{ return m_hThread != NULL; }

protected:
	//����
	virtual DWORD Run()=0;			// �����F�X���b�h���ŌĂ΂�� //

	//�C�x���g
	virtual void OnThreadBegin(){}	// �����F�X���b�h���ŌĂ΂�� //
	virtual void OnThreadEnd(){}	// �����F�X���b�h���ŌĂ΂�� //

private:
	CThread**	m_ppcThread;
};
