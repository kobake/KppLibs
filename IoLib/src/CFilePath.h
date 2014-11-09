#pragma once

#include <stdlib.h> //_MAX_PATH
#include <string>

class CFilePath_Base{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFilePath_Base(const wchar_t* fpath, wchar_t cDelimiter);
	CFilePath_Base(const wchar_t* fdir,const wchar_t* ftitle, wchar_t cDelimiter);
	virtual ~CFilePath_Base();
	void SetDelimiter(wchar_t cDelimiter);

	//�^�ϊ�
//	operator const wchar_t*() const{ return GetPath(); }

	//���
	void Assign(const CFilePath_Base& f);
	CFilePath_Base(const CFilePath_Base& f){ Assign(f); }
	CFilePath_Base& operator = (const CFilePath_Base& f){ Assign(f); return *this; }

	//�ݒ�
	void SetFullPath(const wchar_t* fpath){ SetPath(fpath); } // �b��Alias ###�폜�\��
	void SetDirPath(const wchar_t* fdir){ SetDir(fdir); } // �b��Alias ###�폜�\��
	void SetPath(const wchar_t* fpath);
	void SetPath(const wchar_t* fdir,const wchar_t* ftitle);
	void SetDir(const wchar_t* fdir);
	void SetTitle(const wchar_t* ftitle);
	void SetExt(const wchar_t* ext);

	//�擾
	const wchar_t* GetFullPath() const { return GetPath(); } // �b��Alias ###�폜�\��
	const wchar_t* GetDirPath() const { return GetDir(); } // �b��Alias ###�폜�\��
	const wchar_t* GetPath() const;
	const wchar_t* GetDir() const;
	const wchar_t* GetTitle() const;
	const wchar_t* GetExt() const;	//�u.txt�v�̂悤�Ȍ`��

	std::wstring GetTitleWithoutExt() const;
	std::wstring GetDrive() const;

protected:
	//��؂��
	void _UpdateDelimiter();

private:
	wchar_t					m_cDelimiter;		//��؂蕶���B�u\�vor�u/�v

	wchar_t					m_path[_MAX_PATH];
	wchar_t*				m_delimiter;		//�f�B���N�g���ƃt�@�C���̋��ځB�f�B���N�g���������ꍇ��NULL�B
	wchar_t*				m_extpos;			//�t�@�C���Ɗg���q�̋��ځB�g���q�������ꍇ��NULL�B

	//�L���b�V��
	mutable std::wstring	m_strDirCache;

};

class CFilePathWin : public CFilePath_Base{
public:
	CFilePathWin(const std::wstring& strPath) : CFilePath_Base(strPath.c_str(), L'\\') { }
	CFilePathWin(const wchar_t* fpath=L"") : CFilePath_Base(fpath, L'\\') { }
	CFilePathWin(const wchar_t* fdir,const wchar_t* ftitle) : CFilePath_Base(fdir, ftitle, L'\\') { }
};

class CFilePathUnix : public CFilePath_Base{
public:
	CFilePathUnix(const std::wstring& strPath) : CFilePath_Base(strPath.c_str(), L'/') { }
	CFilePathUnix(const wchar_t* fpath=L"") : CFilePath_Base(fpath, L'/') { }
	CFilePathUnix(const wchar_t* fdir,const wchar_t* ftitle) : CFilePath_Base(fdir, ftitle, L'/') { }
};

typedef CFilePathWin CFilePath;
