#pragma once

#include <string>
#include <vector>
#include "CFilePath.h"

namespace IoLib{

class CFile{
public:
	CFile() : m_cPath(L"") { }
	CFile(const CFilePath& cPath) : m_cPath(cPath) { }
	CFile(const wchar_t* szDir, const wchar_t* szTitle) : m_cPath(szDir, szTitle) { }
	//�ǂݏ���
	void ReadFileBytes(std::vector<unsigned char>* pvBuf) const;
	//�����l
	bool	IsDirectory() const;
	bool	IsFile() const;
	bool	IsExist() const;
	//�p�X
	bool IsValid() const{ return wcslen(m_cPath.GetPath())>=1; }
	const wchar_t* GetPath() const{ return m_cPath.GetPath(); }
	const CFilePath& GetPathClass() const{ return m_cPath; }
	//�e�푮��
	__int64 GetFileSize() const;
	time_t	GetCreatedTime() const;		//�쐬����
	time_t	GetAccessedTime() const;	//�A�N�Z�X����
	time_t	GetModifiedTime() const;	//�X�V����
protected:
	//�����⏕
	void _AdjustPath(); //�p�X���́u..�v����сu.�v������
private:
	CFilePath m_cPath;
//	std::wstring m_strPath;
};

}
using namespace IoLib;

#if 0

#include "CFilePath.h"

class File{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	File(const char* _path="") : path(_path) { }
	File(const char* _dir,const char* _title) : path(_dir,_title) { }
	explicit File(const CFilePath& _path) : path(_path) { }
	virtual ~File(){ }
	//�쐬�E�j��
	void create(int att); //0:�t�@�C�� 1:�t�H���_
	void remove();
	//�ړ��E�R�s�[
	void move(const CFilePath& dst);
	void copy(const CFilePath& dst);
	//���擾
	bool exist();
	int GetSize();
	int getAttribute(); //0:�t�@�C�� 1:�t�H���_ -1:�Ȃ�
	//�A�N�Z�X
	void touch();
private:
	CFilePath path;
public:
	// -- -- CFilePath���b�p�[ -- -- //
	//�ݒ�
	void SetPath(const char* _path)						{ return path.SetPath(_path); }
	void SetPath(const char* _dir,const char* _title)	{ return path.SetPath(_dir,_title); }
	void SetDirPath(const char* _dir)						{ return path.SetDirPath(_dir); }
	void SetTitle(const char* _title)					{ return path.SetTitle(_title); }
	void SetExt(const char* _ext)						{ return path.SetExt(_ext); }
	//�擾
	const char* GetPath() const							{ return path.GetPath(); }
	const char* GetDir() const							{ return path.GetDir(); }
	const char* GetTitle() const						{ return path.GetTitle(); }
	const char* GetExt() const							{ return path.GetExt(); }
public:
	// -- -- ��O -- -- //
	class touch_failed : public std::exception{
	public:
		const char* what() const{ return "File::touch_failed"; }
	};
};


#endif
