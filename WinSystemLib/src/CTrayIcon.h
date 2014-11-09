#pragma once

#include <windows.h>

class CTrayIcon{
public:
	// �������ƏI��
	CTrayIcon();
	CTrayIcon(
		HWND	hwnd,		//!< �ʒm���󂯎��E�B���h�E�n���h��
		UINT	nId,		//!< �^�X�N�g���C������ID (�A�v�����Ń��j�[�N�ł���Ηǂ�)
		UINT	nMsgId,		//!< �ʒm���b�Z�[�WID
		HICON	hIcon,		//!< �A�C�R���C���[�W
		LPCWSTR	szTipText	//!< �`�b�v�e�L�X�g
	);
	virtual ~CTrayIcon();
	void Create();
	void Create(
		HWND	hwnd,		//!< �ʒm���󂯎��E�B���h�E�n���h��
		UINT	nId,		//!< �^�X�N�g���C������ID (�A�v�����Ń��j�[�N�ł���Ηǂ�)
		UINT	nMsgId,		//!< �ʒm���b�Z�[�WID
		HICON	hIcon,		//!< �A�C�R���C���[�W
		LPCWSTR	szTipText	//!< �`�b�v�e�L�X�g
	);
	void Destroy();

private:
	HWND			m_hwnd;
	UINT			m_uiId;
	UINT			m_uiMsgId;
	HICON			m_hIcon;
	std::wstring	m_strTipText;
};

