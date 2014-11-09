#pragma once

#include "CRawDialog.h"
#include "CWindowClass.h"
#include <vector>
#include "util/vector_ex.h"

/*
������������������������������
�� CDialogPanel�N���X�ڍא���     ��
������������������������������
 (�폜�^�C�~���O)
 �E�E�B���h�E�����݂����ԂŁA~CDialogPanel���Ă΂��ƁA
 �@�[���I��WM_DESTROY���Ă΂�A�E�B���h�E�͔j������܂��B
 �ECDialogPanel�����݂����ԂŁA�E�B���h�E���j�������ƁAWM_DESTROY���󂯎��A
   CDialogPanel����hwnd�����o��NULL�ɂȂ�܂����ACDialogPanel�C���X�^���X�͑��݂������܂��B
   ���̌�A~CDialogPanel���Ă΂ꂽ�Ƃ��́A���ɃE�B���h�E�͑��݂��Ȃ��̂ŁA
   WM_DESTROY�͌Ă΂ꂸ�A�j���������s���܂���B
*/


class CDialogPanel;


//�E�B���h�E�v���V�[�W���̊֐��|�C���^
//true��Ԃ����ꍇ�́A����ȍ~�̃E�B���h�E�v���V�[�W�����Ă΂��Ȃ�
typedef bool (*DLGPROC_EX)(
	CDialogPanel*	pcWnd,
	UINT			msg,
	WPARAM			wParam,
	LPARAM			lParam
);

//�������������O�ōs���ꍇ�́ASetHWND���Ăяo��
//  ####�������T�u�N���X���͖��l��
class CDialogPanel : public CRawDialog{ //######������CRawDialog����p��������̂����z�����B
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CDialogPanel();
	virtual ~CDialogPanel();

	//�����Ɣj��
	void Create(int nResourceId, HWND hwndParent);
	void Destroy();
	void Attach(HWND hwnd);

	//�J���p
	virtual BOOL OnMessage(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if(m_dlgprocSubclass){
			return (BOOL)::CallWindowProc((WNDPROC)m_dlgprocSubclass, this->GetHWND(), msg, wparam, lparam);
		}
		else{
			return FALSE; //�f�t�H���g�̏����ɔC����
		}
	}

	//�J�X�^�}�C�Y
	void AddProcedure(DLGPROC_EX wndproc){ m_vHookProcs.push_back_unique(wndproc); }
	void RemoveProcedure(DLGPROC_EX wndproc){ m_vHookProcs.erase_value(wndproc); }

	//���Ɛ��C���^�[�t�F�[�X
	void SetExLong(long n);
	long GetExLong() const;


private:
	friend INT_PTR CALLBACK CDialog_DialogProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	INT_PTR _CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam);
private:
	/*
		(�V�����E�B���h�E�N���X��������ꍇ)
			�E�B���h�E�v���V�[�W��: WndProcWindow
			m_dlgprocSubclass:              NULL
		(�T�u�N���X�������ꍇ)
			�E�B���h�E�v���V�[�W��: WndProcWindow
			m_dlgprocSubclass:              ���̃E�B���h�E�N���X�̃E�B���h�E�v���V�[�W��

		�� �Ăяo���t���[ ��
		(�V�X�e��)
		��
		WndProcWindow (WNDPROC)
		��
		���� m_hookprocs (DLGPROC_EX)
		��
		���� OnMessage (virtual)
		�@�@�@��
		  �@�@���� m_dlgprocSubclass (WNDPROC)
		��
		(�V�X�e��)
	*/
	vector_ex<DLGPROC_EX>	m_vHookProcs;
	DLGPROC					m_dlgprocSubclass;

	//���R�f�[�^
	uchar freedata[32];
};





