#pragma once

#include "CRawWindow.h"
#include "CWindowClass.h"
#include <vector>
#include <BaseLib.h>


/*
������������������������������
�� CWindow�N���X�ڍא���     ��
������������������������������
 (�폜�^�C�~���O)
 �E�E�B���h�E�����݂����ԂŁA~CWindow���Ă΂��ƁA
 �@�[���I��WM_DESTROY���Ă΂�A�E�B���h�E�͔j������܂��B
 �ECWindow�����݂����ԂŁA�E�B���h�E���j�������ƁAWM_DESTROY���󂯎��A
   CWindow����hwnd�����o��NULL�ɂȂ�܂����ACWindow�C���X�^���X�͑��݂������܂��B
   ���̌�A~CWindow���Ă΂ꂽ�Ƃ��́A���ɃE�B���h�E�͑��݂��Ȃ��̂ŁA
   WM_DESTROY�͌Ă΂ꂸ�A�j���������s���܂���B
*/

class CWindowClass;
class CWindow;

struct SWindowInfo{
	//�K�{����
	CWindowClass wclass;
	DWORD ex_style;
	DWORD style;
	const wchar_t* caption;
	const math::CRect rc;
	HWND hwndParent;

	//�I�v�V����
	HMENU hMenu;

	SWindowInfo(
		const CWindowClass&	_wclass,
		DWORD				_ex_style,
		DWORD				_style,
		const wchar_t*		_caption,
		const math::CRect&		_rc,
		HWND				_hwndParent,
		HMENU				_hMenu = NULL
	)
	: wclass(_wclass), ex_style(_ex_style), style(_style), caption(_caption), rc(_rc), hwndParent(_hwndParent), hMenu(_hMenu)
	{
	}
	SWindowInfo()
	: wclass()
	{
	}
	bool valid() const{ return wclass.IsValid(); }
};

//�E�B���h�E�v���V�[�W���̊֐��|�C���^
//true��Ԃ����ꍇ�́A����ȍ~�̃E�B���h�E�v���V�[�W�����Ă΂��Ȃ�
typedef bool (*WNDPROC_EX)(
	CWindow*	pcWnd,
	UINT		msg,
	WPARAM		wParam,
	LPARAM		lParam,
	LRESULT*	pnResult
);

//�������������O�ōs���ꍇ�́ASetHWND���Ăяo��
//  ####�������T�u�N���X���͖��l��
class CWindow : public CRawWindow{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CWindow();
	virtual ~CWindow();

	//�����Ɣj��
	void Create(const SWindowInfo& info);
	void Destroy();
	void Attach(HWND hwnd);

	//�J���p
	virtual LRESULT OnMessage(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if(m_wndprocSubclass){
			return ::CallWindowProc(m_wndprocSubclass, this->GetHWND(), msg, wparam, lparam);
		}
		else{
			return ::DefWindowProc(this->GetHWND(), msg, wparam, lparam);
		}
	}

	//�J�X�^�}�C�Y
	void AddProcedure(WNDPROC_EX wndproc){ m_vHookProcs.push_back_unique(wndproc); }
	void RemoveProcedure(WNDPROC_EX wndproc){ m_vHookProcs.erase_value(wndproc); }

	//Windows�C���^�[�t�F�[�X (###���̂�����͕ʃN���X�Ɍp�����ċ@�\�����������Ƃ���)
	virtual void SetTransparentColor(COLORREF _transparent_color);
	virtual void SetAlpha(uchar _alpha);
	virtual uchar GetAlpha() const{ return m_btAlpha; }
	virtual COLORREF GetTransparentColor() const{ return m_clrTransparent; }
	
	//���Ɛ��C���^�[�t�F�[�X
	void SetExLong(long n);
	long GetExLong() const;

private:
	friend LRESULT CALLBACK WndProcWindow(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	LRESULT _CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam);
private:
	/*
		(�V�����E�B���h�E�N���X��������ꍇ)
			�E�B���h�E�v���V�[�W��: WndProcWindow
			m_wndprocSubclass:              NULL
		(�T�u�N���X�������ꍇ)
			�E�B���h�E�v���V�[�W��: WndProcWindow
			m_wndprocSubclass:              ���̃E�B���h�E�N���X�̃E�B���h�E�v���V�[�W��

		�� �Ăяo���t���[ ��
		(�V�X�e��)
		��
		WndProcWindow (WNDPROC)
		��
		���� m_hookprocs (WNDPROC_EX)
		��
		���� OnMessage (virtual)
		�@�@�@��
		  �@�@���� m_wndprocSubclass (WNDPROC)
		��
		(�V�X�e��)
	*/
	vector_ex<WNDPROC_EX>	m_vHookProcs;
	WNDPROC					m_wndprocSubclass;

	//Windows�ݒ�ێ�
	COLORREF m_clrTransparent;
	uchar m_btAlpha;

	//���R�f�[�^
	uchar m_freedata[32];
};
