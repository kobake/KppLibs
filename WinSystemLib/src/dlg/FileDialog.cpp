#include <BaseLib.h>
#include "FileDialog.h"
#include <StringLib.h>
#include <shlobj.h>
using namespace util;

FileDialog::FileDialog(wchar_t *_filter,wchar_t *_defext)
{
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=NULL; //���
	ofn.hInstance=NULL;
	ofn.lpstrFilter=filter; memcpy(filter,_filter,sizeof(filter));
	ofn.lpstrCustomFilter=NULL; //������
	ofn.nMaxCustFilter=0;       //������
	ofn.nFilterIndex=0;         //������
	ofn.lpstrFile=NULL;         //���
	ofn.nMaxFile=_MAX_PATH-1;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;   //���
	ofn.lpstrTitle=NULL;        //���
	ofn.Flags=0;                //���
	ofn.nFileOffset=0;          //������
	ofn.nFileExtension=0;       //������
	ofn.lpstrDefExt=NULL; if(_defext!=NULL && *_defext!='\0')ofn.lpstrDefExt=wcsdup(_defext);
	ofn.lCustData=0;            //������
	ofn.lpfnHook=NULL;          //������
	ofn.lpTemplateName=NULL;    //������
}

FileDialog::~FileDialog()
{
	free((void*)ofn.lpstrDefExt);
}

bool FileDialog::showOpen(const wchar_t* caption,HWND owner)
{
	int test=sizeof(FileDialog);

	//�f�t�H���g�̃t�@�C����
	wchar_t buf[_MAX_PATH];
	wcssafecpy(buf,fpath.GetPath(),countof(buf));
	
	//�f�t�H���g�̃f�B���N�g��
	if(*fpath.GetDir()){
		wcssafecpy(initialdir,fpath.GetDir(),countof(initialdir));
	}else{
		SHGetSpecialFolderPath(NULL,initialdir,CSIDL_PERSONAL,FALSE); //�}�C�h�L�������g
	}
	
	//���̑��ݒ�
	ofn.lpstrFile=buf;
	ofn.lpstrTitle=caption;
	ofn.hwndOwner=owner;
	ofn.Flags=OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_PATHMUSTEXIST;

	//�_�C�A���O�\��
	BOOL ret=GetOpenFileName(&ofn);

	//CFilePath�ɕϊ�
	if(ret){
		fpath.SetPath(buf);
	}

	return ret!=FALSE;
}

bool FileDialog::showSave(const wchar_t* caption,HWND owner)
{
	//�f�t�H���g�̃t�@�C����
	wchar_t buf[_MAX_PATH];
	wcssafecpy(buf,fpath.GetPath(),countof(buf));

	//�f�t�H���g�̃f�B���N�g��
	if(*fpath.GetDir()){
		wcssafecpy(initialdir,fpath.GetDir(),countof(initialdir));
	}else{
		SHGetSpecialFolderPath(NULL,initialdir,CSIDL_PERSONAL,FALSE); //�}�C�h�L�������g
	}

	//���̑��ݒ�
	ofn.lpstrFile=buf;
	ofn.lpstrTitle=caption;
	ofn.hwndOwner=owner;
	ofn.Flags=OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

	//�_�C�A���O�\��
	BOOL ret=GetSaveFileName(&ofn);

	//CFilePath�ɕϊ�
	if(ret){
		fpath.SetPath(buf);
	}

	return ret!=FALSE;
}
