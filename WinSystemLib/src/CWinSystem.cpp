#include "_required.h"
#include "CWinSystem.h"
#include "mymsg.h"
#include <assert.h>
using namespace std;
HACCEL CWinSystem::m_hAccel;
std::set<HWND> CWinSystem::m_setDialogs;
HWND CWinSystem::m_hwndMain;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �E�B���h�E�N���X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
ATOM CWinSystem::RegisterWindowClass(
	const wchar_t* classname,
	WNDPROC wndproc,
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance)
{
	ATOM ret=0;
	if(hPrevInstance==NULL){
		WNDCLASSEX wndclass;
		wndclass.cbSize=sizeof(WNDCLASSEX);
		wndclass.style=CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc=wndproc;
		wndclass.cbClsExtra=0;
		wndclass.cbWndExtra=0;
		wndclass.hInstance=hInstance;
		wndclass.hIcon=NULL;
		wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
		wndclass.lpszMenuName=NULL;
		wndclass.lpszClassName=classname;
		wndclass.hIconSm=NULL;
		ret=RegisterClassEx(&wndclass);
	}
	return ret;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     ���b�Z�[�W���[�v                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int CWinSystem::DoMessageLoop_Simple(VOID_FUNC PreIdle)
{
	assert(CWinSystem::GetMainWindow());

	MSG msg;
	while(GetMessage(&msg,NULL,0,0)){
		//���A�C�h������ (#######FsWalker��p�B�v����)
		if(PreIdle){
			PreIdle();
		}

		//�A�N�Z�����[�^����
		if(m_hAccel && m_hwndMain){
			if(TranslateAccelerator(m_hwndMain,m_hAccel,&msg))continue;
		}

		//�_�C�A���O����
		set<HWND>::const_iterator p = m_setDialogs.begin(), q = m_setDialogs.end();
		while(p!=q){
			if(::IsDialogMessage(*p, &msg))goto next;
			p++;
		}

		//�ʏ폈��
		TranslateMessage(&msg);
		DispatchMessage(&msg);
next:
		;
	}
	return (int)msg.wParam;
}

int CWinSystem::DoMessageLoop_Modal(HWND hwndDialog,HWND hwndOwner)
{
	//�Ăяo����������
	HWND hwndRoot=GetAncestor(hwndOwner,GA_ROOT);
	EnableWindow(hwndRoot,FALSE);

	//���z���g�͂����Ń_�C�A���O�쐬

	//�|�b�v�A�b�v��������܂ő҂�
	MSG msg;
	while(1){
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			//�A�v���I���ʒm
			if(msg.message==WM_QUIT)goto next;

			//�_�C�A���O�I���ʒm
			if(msg.message==WM_APP_DIALOG_END){
				goto next;
			}

			//���b�Z�[�W�]��
			if(!IsDialogMessage(hwndDialog,&msg)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		//�\��O�F�_�C�A���O���j�����ꂽ
		if(!IsWindow(hwndDialog))break;

		Sleep(10);
	}
next:

	//�Ăяo�����L����
	EnableWindow(hwndRoot,TRUE);
	SetForegroundWindow(hwndRoot);
	SetActiveWindow(hwndRoot);

	//�_�C�A���O�j��
	if(IsWindow(hwndDialog)){
		DestroyWindow(hwndDialog);
	}

	//###���̂Ƃ���͏��0��Ԃ�
	return 0;
}

