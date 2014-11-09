#pragma once

#include <windows.h> //HWND,DWORD,COLORREF
#include "../../../MathLib/MathLib.h" //CRect
#include <string> //wstring

//�������������O�ōs���ꍇ�́ASetHWND���Ăяo��
/*
	CRawWindow���̂̓R�s�[���\�B�i���������̏ꍇ�A�|�����[�t�B�Y���͎����j
	�P����HWND�̑���Ƃ��ĐU�����B

	###��������CRawWindow���̂Ƀ|�����[�t�B�Y���̋@�\(virtual)�͕s�v�����B
*/
class CRawWindow{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CRawWindow(HWND _hwnd=NULL) : m_hwnd(_hwnd) { }
	virtual ~CRawWindow(){}

	//�E�B���h�E�n���h��
	void SetHWND(HWND _hwnd){ m_hwnd=_hwnd; }
	HWND GetHWND() const{ return m_hwnd; }

	//Windows�C���^�[�t�F�[�X
	virtual void Show();
	virtual void SetRect(const math::CRect& rc);
	virtual void SetVisible(bool b);
	virtual void Redraw();
	virtual void GetWindowRect(math::CRect* rc) const;
	virtual void GetWindowRect_OnParent(math::CRect* rc) const;
	virtual void GetClientRect(math::CRect* rc) const;
	virtual void Move(const math::CRect& rc);
	virtual void SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	virtual void KillTimer(UINT_PTR uIDEvent);
	virtual void MessageBox(LPCTSTR lpText, LPCTSTR lpCaption=NULL, UINT uType=MB_OK);
	virtual void SetFocus();
	virtual void SetText(const wchar_t* szText);
	virtual std::wstring GetText() const;
	virtual int GetTextLength() const;
	virtual void SetMenu(HMENU hMenu);

	//������
	virtual void SetFont(const wchar_t* szFace, int nPoint);
	virtual void SetFont(HFONT hFont);
	virtual void SetIcon(HICON hIcon);

	//Windows�C���^�[�t�F�[�X���b�v
	math::CRect GetWindowRect() const{ math::CRect rc; GetWindowRect(&rc); return rc; }
	math::CRect GetWindowRect_OnParent() const{ math::CRect rc; GetWindowRect_OnParent(&rc); return rc; }
	math::CRect GetClientRect() const{ math::CRect rc; GetClientRect(&rc); return rc; }

	//����C���^�[�t�F�[�X
	void MoveToCenter();			//!<��ʒ��S�Ɉړ�
	void SendSizeMessage();			//!<WM_SIZE�𖾎��I�ɔ��s
	void PostSizeMessage();			//!<WM_SIZE�𖾎��I�ɔ��s
	void SetPos(int nX, int nY);	//!<�T�C�Y��ێ����Ĉʒu�̂ݕύX
	void PostCommand(int nId);		//!<WM_COMMAND���[�����s

	//���b�Z�[�W�����C���^�[�t�F�[�X
	LRESULT SendMessage  (UINT msg,WPARAM wparam,LPARAM lparam) const{ return ::SendMessage  (m_hwnd,msg,wparam,lparam); }
	BOOL    PostMessage  (UINT msg,WPARAM wparam,LPARAM lparam) const{ return ::PostMessage  (m_hwnd,msg,wparam,lparam); }
	LRESULT DefWindowProc(UINT msg,WPARAM wparam,LPARAM lparam){ return ::DefWindowProc(m_hwnd,msg,wparam,lparam); }

private:
	HWND m_hwnd;
};
