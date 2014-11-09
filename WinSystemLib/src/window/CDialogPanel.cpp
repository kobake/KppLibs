#include "_required.h"
#include "CDialogPanel.h"
#include "CWindowClass.h"
#include "../CWinError.h"
#include "../Cast.h"
#include "Screen.h"
#include "../../../GraphicsLib/src/CFont.h"
#include "../Cast.h"
#include <DebugLib.h>


//WM_INITDIALOG���ɃC���X�^���X�|�C���^�l��񋟂���
CDialogPanel* g_instant_pCreatingInstance;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  �E�B���h�E�v���V�[�W��                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

INT_PTR CDialogPanel::_CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam)
{
	for(int i=0;i<(int)this->m_vHookProcs.size();i++){
		LRESULT nResult = 0;
		if(this->m_vHookProcs[i](this,msg,wparam,lparam)){
			return TRUE;
		}
	}
	return this->OnMessage(msg,wparam,lparam);
}

/*
�ʏ�A�_�C�A���O�{�b�N�X�v���V�[�W����
���b�Z�[�W�����������ꍇ�� 0 �ȊO�̒l�iTRUE�j���A
�������Ȃ������ꍇ�� 0�iFALSE�j��Ԃ��ׂ��ł��B
�_�C�A���O�{�b�N�X�v���V�[�W���� 0�iFALSE�j��Ԃ����ꍇ�A
�_�C�A���O�}�l�[�W���͂��̃��b�Z�[�W�ɉ������Ċ���̃_�C�A���O���������s���܂��B
*/
INT_PTR CALLBACK CDialog_DialogProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	CDialogPanel* dialog=LongToPointer<CDialogPanel*>(GetWindowLong(hwnd,GWL_USERDATA));
		if(msg==WM_CREATE){
			int n;
			n=0;
		}
	if(dialog){
		LRESULT ret=dialog->_CallProcedures(msg,wparam,lparam);
		if(msg==WM_DESTROY){ //�����́A�uX�v�{�^���ŕ����ꍇ�����A�Ă΂��B
			//hwnd�͖����ɂȂ�̂ŁA�N���A���Ă���
			dialog->SetHWND(NULL);
		}
		return ret;
	}
	else{
		if(msg==WM_INITDIALOG){
			//CDialogPanel*��ێ�
			CDialogPanel* dialog = g_instant_pCreatingInstance;
			::SetWindowLong(hwnd,GWL_USERDATA,PointerToLong(dialog));
			g_instant_pCreatingInstance = NULL;

			if(dialog){
				//HWND�������ŕێ�
				dialog->SetHWND(hwnd);

				//WM_INITDIALOG�𑗂�Ȃ���
				dialog->_CallProcedures(msg,wparam,lparam);
			}
			return TRUE;
		}
	}
	return FALSE;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       �������ƏI��                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CDialogPanel::CDialogPanel()
: m_dlgprocSubclass(NULL)
{
}

CDialogPanel::~CDialogPanel()
{
	Destroy();
}

//#######������
void CDialogPanel::Attach(HWND hwnd)
{
	assert(GetHWND()==NULL);
	this->SetHWND(hwnd);

	//Window*�ݒ�
	LONG nOld = ::SetWindowLong(GetHWND(),GWL_USERDATA,PointerToLong(this));

	//�E�B���h�E�v���V�[�W���ݒ�
	m_dlgprocSubclass = LongToPointer<DLGPROC>(::SetWindowLong(GetHWND(), DWL_DLGPROC, PointerToLong(CDialog_DialogProc)));
}

void CDialogPanel::Create(int nResourceId, HWND hwndParent)
{
	if(GetHWND()==NULL){
		//�E�B���h�E�쐬
		g_instant_pCreatingInstance = this; //###������Ɖ������ǁA�ꎞ�I�ɃO���[�o���ϐ��� this ��ۑ�
		HWND hwnd_tmp = ::CreateDialog(
			c2lib::GetInstance(),
			MAKEINTRESOURCE(nResourceId),
			hwndParent,
			CDialog_DialogProc
		);
		SetHWND(hwnd_tmp);
		//�E�B���h�E�\�� (Panel�Ȃ̂ŁA���\�������Ⴄ)
		this->Show();
	}
}

void CDialogPanel::Destroy()
{
	if(GetHWND()){
		//CDialogPanel*�͖����ɂȂ�̂ŁA���[�U�[�f�[�^���폜���Ă���
		::SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)0);

		//�����I�ɒʒm�𑗂�
		this->_CallProcedures(WM_DESTROY,0,0);

		//�{���ɍ폜
		::DestroyWindow(GetHWND());                      //����ɂ��A�ʒm�͑����Ȃ�
		SetHWND(NULL);
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  ���Ɛ��C���^�[�t�F�[�X                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDialogPanel::SetExLong(long n)
{
	memcpy(freedata,&n,sizeof(n));
}

long CDialogPanel::GetExLong() const
{
	long n;
	memcpy(&n,freedata,sizeof(n));
	return n;
}



