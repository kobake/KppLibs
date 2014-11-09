#pragma once

#include "CFilePath.h"
#include <windows.h>

class FileFind{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	FileFind();
	~FileFind();
	//�C���^�[�t�F�[�X
	bool findFirst(const wchar_t* fdir,const wchar_t* ftitle);
	bool findFirst(const wchar_t* fpath);
	bool findNext();
	void findClose();
	//�擾
	const CFilePath& getFound() const{ return path; }
	bool isFolder() const;

private:
	HANDLE hFind;
	WIN32_FIND_DATA finddata;

	CFilePath path;
};
