#pragma once

#include <string>
#include <IoLib.h> //CFilePath

class CIniSection{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CIniSection(const wchar_t* _fpath, const wchar_t* _section_name);
	~CIniSection();

public:
	//��{���
	const wchar_t*	GetSectionName() const{ return m_strSectionName.c_str(); }	//!<�Z�N�V���������擾

	//�ǎ�
	std::wstring	GetStringEx(const wchar_t* szKey, const wchar_t* def) const;	//!<��������擾�B���ϐ�%�`%��u������
	std::wstring	GetString(const wchar_t* szKey, const wchar_t* def) const;	//!<��������擾
	
	//����
	void			WriteString(const wchar_t* szKey, const wchar_t* szValue);		//!<�������ݒ�

private:
	std::wstring	m_strFilePath;
	std::wstring	m_strSectionName;
};


class CIniFile{
public:
	// �������ƏI�� //
	CIniFile(const wchar_t* _fpath);
	const CFilePath&	GetPath() const{ return m_cFilePath; }

	// �Z�N�V���� //
	int					GetSectionCount() const;
	CIniSection			GetSectionByIndex(int iSectionIndex) const;
	CIniSection			GetSectionByName(const wchar_t* szSectionName) const;

	// �ǎ� //
	std::wstring		GetStringEx(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* def);	//!< ��������擾�B���ϐ�%�`%��u������
	std::wstring		GetString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* def);		//!< ��������擾

	// ���� //
	void				WriteString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szValue);	//!< �������ݒ�

private:
	CFilePath	m_cFilePath;
};


#pragma comment(lib,"comsuppw.lib") //_bstr_t

