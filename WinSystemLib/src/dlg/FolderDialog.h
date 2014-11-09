#pragma once

#include <IoLib.h> // CFilePath
#include <windows.h>

class FolderDialog{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	FolderDialog();
	virtual ~FolderDialog();
	//�C���^�[�t�F�[�X
	bool showSelect(const wchar_t* caption,HWND owner);
	const CFilePath& GetPath() const{ return m_fpath; }
	CFilePath& GetPath(){ return m_fpath; }
private:
	CFilePath m_fpath;
};


