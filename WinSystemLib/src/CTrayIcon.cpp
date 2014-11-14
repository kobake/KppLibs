#include "_required.h"
#include "CTrayIcon.h"
#include <StringLib.h>
#include <ShellAPI.h>
using namespace util;


CTrayIcon::CTrayIcon()
: m_hwnd(NULL)
, m_uiId(0)
, m_uiMsgId(0)
, m_hIcon(NULL)
, m_strTipText(L"")
{
}

CTrayIcon::CTrayIcon(
	HWND	hwnd,		//!< �ʒm���󂯎��E�B���h�E�n���h��
	UINT	nId,		//!< �^�X�N�g���C������ID (�A�v�����Ń��j�[�N�ł���Ηǂ�)
	UINT	nMsgId,		//!< �ʒm���b�Z�[�WID
	HICON	hIcon,		//!< �A�C�R���C���[�W
	LPCWSTR	szTipText	//!< �`�b�v�e�L�X�g
)
{
	Create(hwnd, nId, nMsgId, hIcon, szTipText);
}

CTrayIcon::~CTrayIcon()
{
	Destroy();
}

void CTrayIcon::Create(
	HWND	hwnd,		//!< �ʒm���󂯎��E�B���h�E�n���h��
	UINT	nId,		//!< �^�X�N�g���C������ID (�A�v�����Ń��j�[�N�ł���Ηǂ�)
	UINT	nMsgId,		//!< �ʒm���b�Z�[�WID
	HICON	hIcon,		//!< �A�C�R���C���[�W
	LPCWSTR	szTipText	//!< �`�b�v�e�L�X�g
)
{
	m_hwnd = hwnd;
	m_uiId = nId;
	m_uiMsgId = nMsgId;
	m_hIcon = hIcon;
	m_strTipText = szTipText;
	Create();
}

void CTrayIcon::Create()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uFlags=0;

	//����
	nid.hWnd = m_hwnd;
	nid.uID = m_uiId;    //�^�X�N�g���C��ID(�A�v�����Ń��j�[�N�ł���Ηǂ�)

	//�R�[���o�b�N���b�Z�[�W
	if(m_uiMsgId){
		nid.uFlags |= NIF_MESSAGE;
		nid.uCallbackMessage = m_uiMsgId;
	}
	
	//�A�C�R��
	if(m_hIcon){
		nid.uFlags |= NIF_ICON;
		nid.hIcon = m_hIcon;
	}

	//�`�b�v�e�L�X�g
	if(m_strTipText.length()){
		nid.uFlags |= NIF_TIP;
		wcssafecpy(nid.szTip, m_strTipText.c_str(), countof(nid.szTip)); //�`�b�v�e�L�X�g
	}

	/*
		�ďo

		�o�^�ł���܂ŌJ��Ԃ�
		http://www31.ocn.ne.jp/~yoshio2/vcmemo17-1.html
	*/
	while(1){
		BOOL b = Shell_NotifyIcon(NIM_ADD, &nid);
		if(b){
			break;
		}
		else{
			// �^�C���A�E�g�ȊO�̃G���[����������߂� //
			if(::GetLastError() != ERROR_TIMEOUT){
				return;
			}

			// ���̂��o�^�ł��Ă����疳��� //
			if(Shell_NotifyIcon(NIM_MODIFY, &nid)){
				return;
			}

			// ���g���C�܂ł̑҂����� //
			::Sleep(1000);
		}
	}
}

void CTrayIcon::Destroy()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uFlags = 0;

	//����
	nid.hWnd = m_hwnd;
	nid.uID = m_uiId;    //�^�X�N�g���C��ID(�A�v�����Ń��j�[�N�ł���Ηǂ�)

	//�ďo
	Shell_NotifyIcon(NIM_DELETE, &nid);
}
