#include "common/_required.h"
#include "FolderDialog.h"
#include <StringLib.h>
using namespace util;
#include <shlobj.h>

FolderDialog::FolderDialog()
{
}

FolderDialog::~FolderDialog()
{
}


int CALLBACK DirSelectDialog_Callback(HWND hwnd,UINT msg,LPARAM lParam,LPARAM lpData)
{
	if(msg==BFFM_INITIALIZED){
		SendMessage(hwnd,BFFM_SETSELECTION,(WPARAM)TRUE,lpData);
	}
	return 0;
}

bool FolderDialog::showSelect(const wchar_t* caption,HWND owner)
{
	//�\���̏���
	wchar_t tmp[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner=owner;
	bi.pidlRoot=NULL; //�c���[�̃��[�g
	bi.pszDisplayName=tmp; //�t�H���_��(���Ƃ��΁u���[�J���f�B�X�N(C:)�v�Ȃ�)
	bi.lpszTitle=caption;
	bi.ulFlags=BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS;
	bi.lpfn=DirSelectDialog_Callback;
	bi.lParam=(LPARAM)m_fpath.GetDir();
	bi.iImage=0;

	//�A���P�[�^�擾
	LPMALLOC pMalloc;
	SHGetMalloc(&pMalloc);

	//�_�C�A���O�\��
	ITEMIDLIST* ret=SHBrowseForFolder(&bi);

	if(ret!=NULL){
		//�t�H���_�p�X���擾
		wchar_t buf[_MAX_PATH];
		SHGetPathFromIDList(ret,buf);

		//CFilePath�ɕϊ�
		wcssafecat(buf,L"\\",countof(buf));
		m_fpath.SetPath(buf);
	}

	//�߂�l���
	if(ret)pMalloc->Free(ret);

	//�A���P�[�^���
	pMalloc->Release();
	
	//��
	return ret!=NULL;
}

