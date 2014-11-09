#pragma once

#include <windows.h>

class CClipboard{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CClipboard(HWND hwnd)						{ m_bIsValid = (::OpenClipboard(hwnd)!=FALSE); }
	~CClipboard()								{ Close(); }
	//�C���^�[�t�F�[�X
	bool	IsValid() const						{ return m_bIsValid; }
	operator bool() const						{ return m_bIsValid; }
	//����C���^�[�t�F�[�X
	void	Empty()								{ ::EmptyClipboard(); }
	void	SetData(UINT uFormat, HANDLE hMem)	{ ::SetClipboardData(uFormat, hMem); }
	void	Close()								{ if(m_bIsValid)::CloseClipboard(); m_bIsValid = false; }
	//�擾�C���^�[�t�F�[�X
	HANDLE	GetData(UINT uFormat)				{ return ::GetClipboardData(uFormat); }
	//�C���^�[�t�F�[�X�i���̂����폜�j
	/*
	void setText(const wchar_t* text); //!<�e�L�X�g��ݒ肷��
	wchar_t *getText();          //!<�e�L�X�g���擾����B���݂��Ȃ��ꍇ�� NULL ��Ԃ��B
	*/

	static UINT	S_GetCustomFormat(const wchar_t* szFormatName);
	static UINT S_CF_PREFERREDDROPEFFECT(); //���R�s�[�����ݒ肷�����
	static UINT S_CF_PERFORMEDDROPEFFECT(); //���R�s�[�悪�ݒ肷�����
	static UINT S_CF_PASTESUCCEEDED();
	static UINT S_CF_FILEDESCRIPTOR();
	static UINT S_CF_FILECONTENTS();
private:
	bool	m_bIsValid;
};
