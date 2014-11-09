#pragma once

#include <vector>
#include <string>
#include <wininet.h> //HINTERNET
#include <winsock2.h> //windows.h���O�ɂȂ��ƃ_��
#include <windows.h> //WIN32_FIND_DATA

class CIpAddress;

class CFtp{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFtp();
	CFtp(const CIpAddress& ip,const wchar_t* user,const wchar_t* pass);
	~CFtp();
	//�ڑ�
	void Connect(const CIpAddress& ip,const wchar_t* user,const wchar_t* pass);
	void Disconnect();
	bool IsConnected() const;
	//�ړ��C���^�[�t�F�[�X
	bool							Chdir(const wchar_t* dir);			//!< FTP�J�����g�f�B���N�g����ݒ�
	std::wstring					GetCurrentDir() const;
	//�擾�C���^�[�t�F�[�X
	std::vector<WIN32_FIND_DATA>	Ls(const wchar_t* wildcard);		//!< FTP�J�����g�f�B���N�g�����̃A�C�e���ꗗ���擾
	void							Download(const wchar_t* ftpfile,const wchar_t* localfile);	//!< ftpfile����localfile�փ_�E�����[�h
	//�����C���^�[�t�F�[�X
	void Rename(const wchar_t* szOldName, const wchar_t* szNewName);	//!< ���O�ύX
	void Upload(const wchar_t* localfile,const wchar_t* ftpfile);		//!< localfile����ftpfile�փA�b�v���[�h
	void Remove(const wchar_t* ftpfile);								//!< �t�@�C���폜
	void Mkdir(const wchar_t* ftpdir);									//!< �f�B���N�g���쐬
private:
	HINTERNET m_hInet;
	HINTERNET m_hHost;
};

#include "_lib_wininet.h"





