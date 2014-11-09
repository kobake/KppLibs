#pragma once

#include <Wininet.h>
#include <string>

#include "EHttpStatusCode.h"
#include <map>
#include <string>

class CHttp{
protected:
	void _final();
public:
	//�^
	typedef std::map<std::wstring, std::wstring> TParams;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CHttp(const wchar_t* url = NULL);
	~CHttp();
	
	//���N�G�X�g
	void RequestGet(const wchar_t* szUrl, const TParams& tParams);
	void RequestPost(const wchar_t* szUrl, const TParams& tParams);

	//�擾�C���^�[�t�F�[�X
	std::wstring GetHeader() const; //���w�b�_
	EHttpStatusCode GetStatusCode() const; //�����R�[�h
	std::string ReadAllA() const;
	std::wstring ReadAll(bool bUtf8 = false) const;
	std::string RequestGet(const wchar_t* server,const wchar_t* url) const; //!< GET���N�G�X�g�̕ԓ����擾����
private:
	HINTERNET m_hInet;
	HINTERNET m_hUrl;
};

#include "_lib_wininet.h"
