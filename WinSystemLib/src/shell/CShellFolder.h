#pragma once

#include <shlobj.h> //IShellFolder
#include <string>   //std::wstring
#include "CItemIdList.h"
#include "shared_shell_ptr.h"
class CEnumIdList;

typedef shared_shell_ptr<IShellFolder> IShellFolderSharedPtr;


/*!
	IShellFolder�̃��b�p�[
	
	���R�s�[�\�B

	�g�p��)
		CShellFolder f(L"::MyComputer");
		CShellFolder f(L"D:");
		CShellFolder f(L"D:\\_tmp");
*/
class CShellFolder{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CShellFolder(const wchar_t* szFolderPath = NULL);
	virtual ~CShellFolder();
	void Attach(IShellFolder* lpShellFolder);
	bool IsValid() const{ return m_lpShellFolder!=NULL; }

	//�C���^�[�t�F�[�X
	CItemIdList		CreateItemIDList(const wchar_t* szPath);
	IShellFolder*	CreateShellFolder(const CItemIdList& pidl);
	std::wstring	GetDisplayNameOfItem(const CItemIdList& cidl) const;
	IContextMenu*	CreateContextMenu(LPCITEMIDLIST* apIDL, int nIDL) const;

	// �� //
	CEnumIdList		CreateEnumIDList() const;

	//�J���p
	IShellFolder* _GetIShellFolder(){ return m_lpShellFolder.get(); }
	const CItemIdList& _GetItemIdList() const{ return m_cidl; }

	//���Z�q
	IShellFolder* operator -> () { return m_lpShellFolder.get(); }

private:
	std::wstring			m_debug;
	std::wstring			m_debug2;
	IShellFolderSharedPtr	m_lpShellFolder;
	CItemIdList				m_cidl;				//!<�A�C�e��ID���X�g�B�f�X�N�g�b�v�̏ꍇ��NULL�B###����Ƃ���g���Ȃ��B
};

//!IShellFolder(SHGetDesktopFolder)�̃��b�v
class CDesktopFolder : public CShellFolder{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CDesktopFolder();
};
