#pragma once

#include <windows.h>
#include <IoLib.h> // CFilePath

class Module{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Module(HINSTANCE _hInst=NULL);
	//�C���^�[�t�F�[�X
	const CFilePath& GetPath() const{ return path; }
private:
	HINSTANCE hInst;
	CFilePath path;
};
