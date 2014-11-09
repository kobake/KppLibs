#include "common/_required.h"
#include <WinSystemLib.h>
#include "../resource.h"

class CMainPanel : public CDialogPanel{
public:
	BOOL OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg){
		case WM_INITDIALOG:
//			this->MessageBox(L"PANEL");
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
			case IDOK:
				this->MessageBox(L"›");
				break;
			case IDCANCEL:
				this->MessageBox(L"~");
				break;
			}
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}
};

CMainPanel cPanel;

class CMainWindow : public CWindow{
public:
	LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg){
		case WM_CREATE:
			cPanel.Create(DLG_MAIN, this->GetHWND());
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		default:
			return __super::OnMessage(msg, wParam, lParam);
		}
		return 0L;
	}
};

CMainWindow cMainWindow;

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
				   LPWSTR lpCmdLine, int nCmdShow)
{
	c2lib::Init(hCurInst, hPrevInst, lpCmdLine, nCmdShow);

	cMainWindow.Create(
		SWindowInfo(
			CWindowClass(c2lib::GetInstance(), L"MainWindow"),
			0,
			WS_OVERLAPPEDWINDOW,
			L"WinSample",
			math::CRect(DEF, DEF, DEF, DEF),
			NULL,
			NULL
		)
	);
	cMainWindow.Show();

	return CWinSystem::DoMessageLoop_Simple();
}
