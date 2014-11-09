#pragma once

#include <windows.h>
#include <set>

typedef void (*VOID_FUNC)();

class CWinSystem{
public:
	//!�E�B���h�E�N���X�o�^
	static ATOM RegisterWindowClass(
		const wchar_t* classname,
		WNDPROC wndproc,
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance
	);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                     ���b�Z�[�W���[�v                        //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

	//!�ʏ�̃��b�Z�[�W���[�v
	/*!
		�ʏ�̃��b�Z�[�W���[�v�BWM_QUIT�����o�������_�ŏ������߂�B

		@ret WM_QUIT���_�ł�MSG::wParam
	*/
	static int DoMessageLoop_Simple(VOID_FUNC PreIdle = NULL);

	//!���[�_���_�C�A���O�p���b�Z�[�W���[�v
	/*!
		�_�C�A���O���I���������_�ŏ������߂�B

		@ret �������B���̂Ƃ�����0��Ԃ��B
	*/
	static int DoMessageLoop_Modal(HWND hwndDialog,HWND hwndOwner);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                   ���b�Z�[�W���[�v�ݒ�                      //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//�A�N�Z�����[�^
	static void SetAccelerator(HACCEL hAccel){ m_hAccel = hAccel; }
	//���C���E�B���h�E
	static void SetMainWindow(HWND hwnd){ m_hwndMain = hwnd; }
	static HWND GetMainWindow(){ return m_hwndMain; }
	//�_�C�A���O�E�B���h�E
	static void AddDialogWindow(HWND hwnd){ if(hwnd)m_setDialogs.insert(hwnd); }
	static void RemoveDialogWindow(HWND hwnd){ if(hwnd)m_setDialogs.erase(hwnd); }
//	static HWND GetDialogWindow(){ return m_hwndDialog; }
private:
	static HACCEL			m_hAccel;
	static HWND				m_hwndMain;
	static std::set<HWND>	m_setDialogs;
};


//�x���������b�v
inline LONG_PTR MySetWindowLongPtr(HWND hwnd, int nIndex, LONG_PTR dwNewLong)
{
	return ::SetWindowLongPtr(hwnd,nIndex,(LONG)dwNewLong);
}
inline LONG_PTR MyGetWindowLongPtr(HWND hwnd, int nIndex)
{
	return ::GetWindowLongPtr(hwnd,nIndex);
}


