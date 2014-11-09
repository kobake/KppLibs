#pragma once

#include <shlobj.h>
#include "shared_shell_ptr.h"
#include <vector>
#include <string>
#include "../CWindowSubclass.h"
class CShellFolder;
class CItemIdList;

typedef shared_shell_ptr<IContextMenu> IContextMenuPtr;

/*!
	IContextMenu�̃��b�v
*/
class CContextMenu : public CWindowSubclass{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CContextMenu(
		const CShellFolder&	cShellFolder,	//!< �t���p�X�t�H���_
		const CItemIdList&	cidlItem		//!< �t�H���_����̑��΃A�C�e��ID���X�g
	);
	CContextMenu(	//����
		const CShellFolder&				cShellFolder,
		const std::vector<CItemIdList>	vItems
	);
	virtual ~CContextMenu();

	//�g�p
	void AppendTo(
		HMENU	hMenu,
		int		nIdMin,
		int		nIdMax,
		bool	bCanRename = false,
		bool	bForDrive = false
	);
	void Invoke(int nId);
	std::wstring GetCommandString(int nId) const;

	//�A�N�Z�X
	IContextMenu* _GetImp(){ return m_lpContextMenu.get(); }
	IContextMenu* operator-> () { return m_lpContextMenu.get(); }

	//�g���C���^�[�t�F�[�X
	IContextMenu2* I2(){ return static_cast<IContextMenu2*>(m_lpContextMenu.get()); }
	IContextMenu3* I3(){ return static_cast<IContextMenu3*>(m_lpContextMenu.get()); }

	//�T�u�N���X�� //��SubclassWindow�Ƃ������O�͊��Ƀ}�N���Ƃ��Ă���̂ŁA�g���Ȃ��B
//	void SetSubclassWindow(HWND hwnd);
//	void UnsetSubclassWindow();
	bool HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lresult); //����������������TRUE��Ԃ�
//	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	IContextMenuPtr	m_lpContextMenu;
	int				m_nType;			// 0 or 1 or 2 or 3
	HWND			m_hwndSubclass;
	WNDPROC			m_wndprocSubclass;
	//
	int				m_nIdMin;	//AppendTo����nIdMin���ꎞ�I�ɋL�^
};
