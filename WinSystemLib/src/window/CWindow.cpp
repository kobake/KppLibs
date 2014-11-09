#include "_required.h"
#include "CWindow.h"
#include "CWindowClass.h"
#include "../CWinError.h"
#include "../Cast.h"
#include "Screen.h"
#include "../../../GraphicsLib/src/CFont.h"
#include <DebugLib.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  �E�B���h�E�v���V�[�W��                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT CWindow::_CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam)
{
	for(int i=0;i<(int)this->m_vHookProcs.size();i++){
		LRESULT nResult = 0;
		if(this->m_vHookProcs[i](this,msg,wparam,lparam,&nResult)){
			return nResult;
		}
	}
	return this->OnMessage(msg,wparam,lparam);
}

LRESULT CALLBACK WndProcWindow(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	CWindow* window=LongToPointer<CWindow*>(GetWindowLong(hwnd,GWL_USERDATA));
	if(window){
		LRESULT ret=window->_CallProcedures(msg,wparam,lparam);
		if(msg==WM_DESTROY){ //�����́A�uX�v�{�^���ŕ����ꍇ�����A�Ă΂��B
			//hwnd�͖����ɂȂ�̂ŁA�N���A���Ă���
			window->SetHWND(NULL);
		}
		return ret;
	}
	else{
		if(msg==WM_CREATE){
			//CWindow*��ێ�
			CREATESTRUCT* cs=(CREATESTRUCT*)lparam;
			CWindow* window=(CWindow*)cs->lpCreateParams;
			::SetWindowLong(hwnd,GWL_USERDATA,PointerToLong(window));

			if(window){
				//HWND�������ŕێ�
				window->SetHWND(hwnd);

				//WM_CREATE�𑗂�Ȃ���
				window->_CallProcedures(msg,wparam,lparam);
			}
		}
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       �������ƏI��                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//###�b��t�H���g
static void _SetTmpFont(HWND hwnd)
{
	static CFont cFont(L"�l�r �S�V�b�N", CPointSize(9));
	::SendMessage(hwnd, WM_SETFONT, (WPARAM)cFont.GetHFONT(), TRUE);
}

CWindow::CWindow()
: m_wndprocSubclass(NULL)
{
	//�ݒ�̃f�t�H���g�l
	m_clrTransparent=0x000000;
	m_btAlpha=255;
}

CWindow::~CWindow()
{
	Destroy();
}

void CWindow::Attach(HWND hwnd)
{
	assert(GetHWND()==NULL);
	this->SetHWND(hwnd);

	//Window*�ݒ�
	LONG nOld = ::SetWindowLong(GetHWND(),GWL_USERDATA,PointerToLong(this));

	//�E�B���h�E�v���V�[�W���ݒ�
	m_wndprocSubclass = LongToPointer<WNDPROC>(::SetWindowLong(GetHWND(), GWL_WNDPROC, PointerToLong(WndProcWindow)));

	//##�b��t�H���g
	_SetTmpFont(this->GetHWND());
}

void CWindow::Create(const SWindowInfo& info)
{
	if(info.valid() && GetHWND()==NULL){
		//�E�B���h�E�N���X�o�^
		bool bRegisted = info.wclass.Regist(WndProcWindow);

		//�E�B���h�E�쐬
		HWND hwnd_tmp=CreateWindowEx(
			info.ex_style,
			info.wclass.GetName(),
			info.caption,
			info.style,
			def_adjust<int>(info.rc.Left(),  CW_USEDEFAULT),	//���W
			def_adjust<int>(info.rc.Top(),   CW_USEDEFAULT),
			def_adjust<int>(info.rc.Width(), CW_USEDEFAULT),
			def_adjust<int>(info.rc.Height(),CW_USEDEFAULT),
			info.hwndParent,									//�e�E�B���h�E
			info.hMenu,											//���j���[
			info.wclass.GetModule(),							//HINSTANCE
			this												//lpParam
		);

		//�E�B���h�E�N���X�����ɓo�^�ς݂̂��̂ł�A
		//���̃E�B���h�E�v���V�[�W���� WndProcWindow �łȂ��ꍇ�́A(�܂�AWindows�W���N���X(EDIT�Ȃ�)�ł���\��������)
		//�T�u�N���X�����s��
		if(!bRegisted && (WNDPROC)(intptr_t)::GetWindowLong(hwnd_tmp,GWL_WNDPROC)!=WndProcWindow){
			Attach(hwnd_tmp);
		}
		else{
			SetHWND(hwnd_tmp);
		}

		//�G���[�`�F�b�N
		if(GetHWND()==NULL){
			int test=1;
			test=0;
	//		WinError().show_error();
		}

		//##�b��t�H���g
		_SetTmpFont(this->GetHWND());
	}
}

void CWindow::Destroy()
{
	if(GetHWND()){
		//CWindow*�͖����ɂȂ�̂ŁA���[�U�[�f�[�^���폜���Ă���
		::SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)0);

		//�����I�ɒʒm�𑗂�
		this->_CallProcedures(WM_DESTROY,0,0);

		//�{���ɍ폜
		::DestroyWindow(GetHWND());                      //����ɂ��A�ʒm�͑����Ȃ�
		SetHWND(NULL);
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �C���^�[�t�F�[�X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void CWindow::SetTransparentColor(COLORREF _transparent_color)
{
	m_clrTransparent=_transparent_color;
	SetLayeredWindowAttributes(GetHWND(),m_clrTransparent,m_btAlpha,LWA_COLORKEY | LWA_ALPHA);
}

void CWindow::SetAlpha(uchar _alpha)
{
	m_btAlpha=_alpha;
	SetLayeredWindowAttributes(GetHWND(),m_clrTransparent,m_btAlpha,LWA_COLORKEY | LWA_ALPHA);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  ���Ɛ��C���^�[�t�F�[�X                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CWindow::SetExLong(long n)
{
	memcpy(m_freedata,&n,sizeof(n));
}

long CWindow::GetExLong() const
{
	long n;
	memcpy(&n,m_freedata,sizeof(n));
	return n;
}

