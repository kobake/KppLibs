#pragma once

#include <IoLib.h> // CFilePath
#include <windows.h>

class FileDialog{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	FileDialog(wchar_t* _filter,wchar_t* _defext);
	virtual ~FileDialog();

	//�_�C�A���O�\��
	bool showOpen(const wchar_t *caption,HWND owner);
	bool showSave(const wchar_t *caption,HWND owner);

	//�擾
	const CFilePath& GetPath() const{ return fpath; }
	CFilePath& GetPath(){ return fpath; }
private:
	OPENFILENAME ofn;
	wchar_t filter[256];
	wchar_t initialdir[_MAX_PATH+2];

	CFilePath fpath;
};


