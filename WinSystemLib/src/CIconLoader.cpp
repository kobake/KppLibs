#include "common/_required.h"
#include "CIconLoader.h"
#include <string>
#include <ShellAPI.h>
using namespace std;

HICON CIconLoader::LoadIconOfFolder()
{
	SHFILEINFO fi;

	//�t���O
	UINT uFlags = 0;
	uFlags |= SHGFI_ICON;			//�A�C�R���n���h�����擾 -> hIcon (���g���I�������A�A�v������DestroyIcon����)
	uFlags |= SHGFI_SMALLICON;		//�������A�C�R��
	uFlags |= SHGFI_SYSICONINDEX;	//�V�X�e���C���[�W���X�g���C���f�b�N�X���擾 -> iIcon
	uFlags |= SHGFI_USEFILEATTRIBUTES;

	//�A�C�R�����̎擾
	DWORD dwShellResult = (DWORD)SHGetFileInfo(
		L"",
		FILE_ATTRIBUTE_DIRECTORY,
		&fi,
		sizeof(fi),
		uFlags
	);
	if(dwShellResult!=0){
		return fi.hIcon;
	}
	else{
		return NULL;
	}
}


HICON CIconLoader::LoadIconOfExt(
	const wchar_t* szExt	//�u.txt�v�̂悤�Ȍ`��
)
{
	SHFILEINFO fi;

	//�t���O
	UINT uFlags = 0;
	uFlags |= SHGFI_ICON;			//�A�C�R���n���h�����擾 -> hIcon (���g���I�������A�A�v������DestroyIcon����)
	uFlags |= SHGFI_SMALLICON;		//�������A�C�R��
	uFlags |= SHGFI_SYSICONINDEX;	//�V�X�e���C���[�W���X�g���C���f�b�N�X���擾 -> iIcon
	uFlags |= SHGFI_USEFILEATTRIBUTES;

	//�A�C�R�����̎擾
	wstring strPath = L"*"; strPath+=szExt;
	DWORD dwShellResult = (DWORD)SHGetFileInfo(
		strPath.c_str(),
		0,
		&fi,
		sizeof(fi),
		uFlags
	);
	if(dwShellResult!=0){
		return fi.hIcon;
	}
	else{
		return NULL;
	}
}


//!�t�@�C���������A�C�R�������[�h
HICON CIconLoader::LoadIconOfFile(const wchar_t* szPath)
{
	SHFILEINFO fi;

	//�t���O
	UINT uFlags = 0;
	uFlags |= SHGFI_ICON;			//�A�C�R���n���h�����擾 -> hIcon (���g���I�������A�A�v������DestroyIcon����)
	uFlags |= SHGFI_SMALLICON;		//�������A�C�R��
	uFlags |= SHGFI_SYSICONINDEX;	//�V�X�e���C���[�W���X�g���C���f�b�N�X���擾 -> iIcon
	uFlags |= SHGFI_ADDOVERLAYS;

	//���h���C�u�̏ꍇ�̓I�[�o�[���C����
	if(wcslen(szPath)<=3){ //####��
		uFlags &= ~SHGFI_ADDOVERLAYS;
	}
	
	//�A�C�R�����̎擾
	DWORD dwShellResult = (DWORD)SHGetFileInfo(
		szPath,
		0,
		&fi,
		sizeof(fi),
		uFlags
	);
	if(dwShellResult!=0){
		return fi.hIcon;
	}
	else{
		return NULL;
	}
}

