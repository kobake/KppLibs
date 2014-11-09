#pragma once

#include "CRawWindow.h"
#include "../CWinSystem.h"

class CRawDialog : public CRawWindow{
public:
	//�E�B���h�E�n���h��
	void SetHWND(HWND hwnd)
	{
		__super::SetHWND(hwnd);
		CWinSystem::AddDialogWindow(hwnd);
	}

	//�_�C�A���O�C���^�[�t�F�[�X
	CRawWindow GetDlgItem(int nId) const
	{
		CRawWindow ret;
		ret.SetHWND(::GetDlgItem(this->GetHWND(), nId));
		return ret;
	}
	BOOL EndDialog(INT_PTR nResult)
	{
		return ::EndDialog(this->GetHWND(), nResult);
	}
};

/*
	CDialog���p�����ĐV�����N���X�����AOnMessage���I�[�o�[���C�h���Ďg��

	class CMyDialog : public CDialog{
	public:
		
	};
*/
class CDialog : public CRawDialog{
public:
	static INT_PTR CALLBACK S_DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	// �������ƏI��
	CDialog();
	~CDialog();
	int CreateModal(int dialogResourceId, HWND hwndParent);
	// �C�x���g
	virtual BOOL OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

