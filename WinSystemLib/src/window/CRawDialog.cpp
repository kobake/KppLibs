#include "common/_required.h"
#include "CRawDialog.h"


#include "CRawDialog.h"
#include <windows.h>
#include <map>

CDialog* g_temporaryDialog = NULL;

INT_PTR CALLBACK CDialog::S_DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CDialog* dialog = (CDialog*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if(!dialog && g_temporaryDialog){
		dialog = g_temporaryDialog;
		g_temporaryDialog = NULL;
		dialog->SetHWND(hwnd);
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)dialog);
	}
	if(dialog){
		INT_PTR ret = dialog->OnMessage(msg, wParam, lParam);
		if(msg == WM_DESTROY){
			dialog->SetHWND(NULL);
		}
		return ret;
	}
	else{
		return FALSE;
	}
}

CDialog::CDialog()
{
}

CDialog::~CDialog()
{
	if(GetHWND()){
		::SetWindowLongPtr(GetHWND(), GWLP_USERDATA, 0);
	}
}

int CDialog::CreateModal(int dialogResourceId, HWND hwndParent)
{
	assert(!g_temporaryDialog);
	g_temporaryDialog = this;
	return ::DialogBox(BaseLib::GetInstance(), MAKEINTRESOURCE(dialogResourceId), hwndParent, S_DialogProc);
}
