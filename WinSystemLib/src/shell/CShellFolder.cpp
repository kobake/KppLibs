#include <BaseLib.h>
#include "CShellFolder.h"
#include "CMalloc.h"
#include <comutil.h> //_bstr_t
#include "CEnumIdList.h"
#include <DebugLib.h>
#pragma comment(lib,"comsuppw.lib") //_bstr_t
using namespace std;


//�f�X�N�g�b�v
CDesktopFolder::CDesktopFolder()
: CShellFolder((const wchar_t*)1)
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  CShellFolder                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CShellFolder::CShellFolder(const wchar_t* szFolderPath)
{
	//����
	if(szFolderPath==NULL){
		m_debug = L"null";
		m_cidl.Attach(NULL);
		m_lpShellFolder = IShellFolderSharedPtr((IShellFolder*)NULL);
	}
	//���X�y�V�����t�H���_ (�f�X�N�g�b�v)
	else if(szFolderPath==(const wchar_t*)1){
		m_debug = L"desktop";
		//IShellFolder�擾
		IShellFolder* psfTmp = NULL;
		SHGetDesktopFolder(&psfTmp);
		m_lpShellFolder = IShellFolderSharedPtr(psfTmp);

		//�A�C�e��ID���X�g���\�z
		LPITEMIDLIST pidl;
		::SHGetSpecialFolderLocation(
			NULL,
			CSIDL_DESKTOP,
			&pidl
		);
		m_cidl.Attach(pidl);
	}
	//�X�y�V�����t�H���_
	else if(szFolderPath[0]==L':' && szFolderPath[1]==L':'){
		m_debug = szFolderPath;
		//�}�C�R���s���[�^
		if(wcscmp(szFolderPath,L"::MyComputer")==0){
			szFolderPath = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}";
			goto other_folder;
		}
		//�R���g���[���p�l��
		else if(wcscmp(szFolderPath,L"::ControlPanel")==0){
			szFolderPath = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{21EC2020-3AEA-1069-A2DD-08002B30309D}";
			goto other_folder;
		}
		//���̑�
		else{
			goto other_folder;
		}
	}
	//���̑��̃t�H���_
	else{
		m_debug = szFolderPath;
other_folder:
		//�f�X�N�g�b�v��p����
		CDesktopFolder desktop;

		//�A�C�e��ID���X�g�擾
		m_cidl = desktop.CreateItemIDList(szFolderPath);

		//###����
		wstring test = m_cidl.GetPath();

		//IShellFolder�擾
		m_lpShellFolder = IShellFolderSharedPtr(desktop.CreateShellFolder(m_cidl));
	}
	m_debug2 = m_cidl.GetPath();
}

void CShellFolder::Attach(IShellFolder* lpShellFolder)
{
//	if(lpShellFolder)lpShellFolder->AddRef(); //###�v����
	m_debug = L"attached";
	if(lpShellFolder){
		m_lpShellFolder = IShellFolderSharedPtr(lpShellFolder);
		m_cidl = this->CreateItemIDList(L""); //###�v����
	}
	else{
		m_lpShellFolder = IShellFolderSharedPtr((IShellFolder*)NULL);
		m_cidl.Clear(); //###�v����
	}
	m_debug2 = m_cidl.GetPath();
}

CShellFolder::~CShellFolder()
{
	//��shared_shell_ptr��Release���Ă����͂�

	//if(m_lpShellFolder){
	//	m_lpShellFolder->Release();
	//}
}

//�C���^�[�t�F�[�X
CItemIdList CShellFolder::CreateItemIDList(const wchar_t* szPath)
{
	LPITEMIDLIST pidl = NULL;
	_bstr_t strPath = szPath;
	HRESULT hr = m_lpShellFolder->ParseDisplayName(
		NULL,
		NULL,
		strPath,
		NULL,
		&pidl,
		NULL
	);
	if(hr!=NOERROR){
		ErrorLog(LEVEL_DEVELOPING_ERROR,L"����������������������������������������\r\n");
		ErrorLog(LEVEL_DEVELOPING_ERROR,L"Critical Error: ParseDisplayName failed.\r\n");
		ErrorLog(LEVEL_DEVELOPING_ERROR,L"����������������������������������������\r\n");
	}
	//�N���X�ɃA�^�b�`
	CItemIdList cidl;
	cidl.Attach(pidl);

	//###����
	wstring test = cidl.GetPath(*this);

	return cidl;
}

IShellFolder* CShellFolder::CreateShellFolder(const CItemIdList& cidl)
{
	IShellFolder* lpShellFolder = NULL;
	HRESULT hRet = m_lpShellFolder->BindToObject(
		cidl._GetImp(),
		NULL,
		IID_IShellFolder,
		(LPVOID*)&lpShellFolder
	);
	if(hRet==NOERROR){
		return lpShellFolder;
	}
	else{
		return NULL;
	}
}

CEnumIdList CShellFolder::CreateEnumIDList() const
{
	IEnumIDList* pEnum;

	//�񋓃t���O
	SHCONTF eFlags = 0;
	eFlags |= SHCONTF_FOLDERS;
	eFlags |= SHCONTF_NONFOLDERS;
	eFlags |= SHCONTF_INCLUDEHIDDEN;
	eFlags |= SHCONTF_INIT_ON_FIRST_NEXT;
//	eFlags |= SHCONTF_NETPRINTERSRCH;
	eFlags |= SHCONTF_SHAREABLE;
	eFlags |= SHCONTF_STORAGE;

	m_lpShellFolder->EnumObjects(NULL,eFlags,&pEnum);
	return CEnumIdList(pEnum);
}

#include <StringLib.h>

std::wstring CShellFolder::GetDisplayNameOfItem(const CItemIdList& cidl) const
{
	STRRET	ret;
	HRESULT hr = m_lpShellFolder->GetDisplayNameOf( cidl._GetImp(), SHGDN_NORMAL, &ret ); //###str.pOleStr���������Ƃ�����

	//API���s
	if(FAILED(hr))
		return L"<? GetDisplayNameOf failed ?>";

	//����
	switch(ret.uType){
	case STRRET_WSTR:	return ret.pOleStr;
	case STRRET_OFFSET:	return (LPCTSTR)((char*)cidl._GetImp() + ret.uOffset);
	case STRRET_CSTR:	return cpp_mbstowcs(ret.cStr);
	}

	//���ގ��s
	return L"<? GetDisplayNameOfItem failed ?>";
}

IContextMenu* CShellFolder::CreateContextMenu(LPCITEMIDLIST* apIDL, int nIDL) const
{
	IContextMenu* lpContextMenu;
	HRESULT hr = m_lpShellFolder->GetUIObjectOf(NULL, nIDL, apIDL, IID_IContextMenu, NULL, (LPVOID*)&lpContextMenu);
	if(FAILED(hr))
		return NULL;
	return lpContextMenu;
}

