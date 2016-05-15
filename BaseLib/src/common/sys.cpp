#include "_required.h"
#include "sys.h"
#include <DebugLib.h>
#include <locale.h>
#include <objbase.h> //CoInitialize
#include <commctrl.h> //InitCommonControls
#include "common/CAppInfo.h"
#include <LogLib.h>
#include <StringLib.h>
#include <Windows.h>
#include <mmsystem.h>
#include <time.h>

static HINSTANCE g_hInst;
static bool g_bInit;

//�R���\�[���A�E�B���h�E�Y�x�[�X�A���ʂ̏���������
static void _init()
{
	//���P�[��������
	//setlocale(LC_ALL,"Japanese");
	setlocale(LC_ALL,"");

	// timeGetTime�̕���\ //
	timeBeginPeriod(1);

#ifdef _DEBUG
	//���������[�N���o�V�X�e��
//	DebugSystem::memoryCheckBegin();
#endif

	//�R�����R���g���[�������� ��comctl32.lib���K�v
	InitCommonControls();
	INITCOMMONCONTROLSEX ic;
	ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ic.dwICC = ICC_COOL_CLASSES; //���o�[
	InitCommonControlsEx(&ic); 

	// ����������
	srand(timeGetTime());

	// ���O������
//	CLogManager::Instance()->Init(*CAppInfo::Instance());
}

//main�p
void BaseLib::Init(int argc,wchar_t* argv[],ASSERT_HANDLE pfAssertHandle)
{
	if(g_bInit)return;
	g_bInit = true;

	_init();
	debugInit(DEBUG_OUT_STDOUT, pfAssertHandle);
}

//WinMain�p
void BaseLib::Init(HINSTANCE hInst,HINSTANCE hPrevInst,LPWSTR lpCmdLine,int nCmdShow,ASSERT_HANDLE pfAssertHandle)
{
	if(g_bInit)return;
	g_bInit = true;

	_init();
	g_hInst=hInst;
	debugInit(DEBUG_OUT_DEBUGGER, pfAssertHandle);
}

//DllMain�p
void BaseLib::Init(HINSTANCE hInst, DWORD fdwReason, PVOID pvReserved,ASSERT_HANDLE pfAssertHandle)
{
	if(g_bInit)return;
	g_bInit = true;

	_init();
	g_hInst=hInst;
	debugInit(DEBUG_OUT_MSGBOX, pfAssertHandle);
}

void BaseLib::InitCom()
{
	//COM������
	HRESULT ret=CoInitialize(NULL);
	assert(ret==S_OK);
}


HINSTANCE BaseLib::GetInstance()
{
	return g_hInst;
}


