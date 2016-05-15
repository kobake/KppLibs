#pragma once

#include "c_Window.h"
#include "c_Accelerator.h"
#include "t_base.h"
#include "c_Screen.h"
#include <WinSystemLib.h> // ini
#include <ShellAPI.h> // NOTIFYICONDATA
#include <vector>
class Icon;
class CBitmap;

int argsGetOptions(int argc,wchar *argv[],int *_iarg,int opt[],NAME_VALUE_PAIR<int> opt_table[]);

#define TASKICON_MAX	16
class App{
private:
	NOTIFYICONDATA *ni_list[TASKICON_MAX];
	Icon *task_icon[TASKICON_MAX];
	HINSTANCE hInst;
	HINSTANCE hPrevInst;
	bool is_dll;
	wstring softname;
	wchar exedir[_MAX_PATH];
	wchar exepath[_MAX_PATH];
public:
	CIniFile *inifile;
	wchar error_msg[256];
	HANDLE hMutex;
	bool multi_run;
	HHOOK hKeyHook;
	int usingWSA;
	WSADATA wsadata;
	SimpleList<VOIDPROC> proc_on_end;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	void _init_v();
	App(HINSTANCE _hInst,HINSTANCE _hPrevInst,const wstring& _softname,const wchar *_mutex=NULL);
	App(const wchar *dllname);
	virtual ~App();
	virtual int init()=0;
	//���\�[�X
	Icon *loadIcon(int id,int w=0,int h=0);
	CBitmap *loadBitmap(int id);
	void *loadResource(int id,int type);
	void loadResource2(std::vector<BYTE>* buf, int id,int type);
	//���E�p�X
	HINSTANCE getInstance(){ return hInst; }
	HINSTANCE getPrevInstance(){ return hPrevInst; }
	wstring getSoftName(){ return wstring(softname); }
	wstring getExeDir(){ return wstring(exedir); }
	wstring getExePath(){ return wstring(exepath); }
	bool multiRun();
	void chDir(const wstring& path);
	void setEnv(const wstring& name,const wstring& value);
	//�G���[����
	void setErrorMessage(wchar *msg);
	wchar *getErrorMessage();
	//���b�Z�[�W����
	int windowsLoop();
	int dialogLoop(Window *wnd,Accelerator *accel=NULL);
	int dialogLoop(HWND hwnd,Accelerator *accel=NULL);
	int acceleratorLoop(Window *wnd,Accelerator *accel);
	int gameLoop(ROUTINE r);
	void postQuit(int code=0);
	//�t�b�N
	void hookKeyboard(HOOKPROC keyproc);
	void unhookKeyboard();
	LRESULT callKeyboardNext(int code,WPARAM wParam,LPARAM lParam);
	//�l�b�g���[�N����
	bool useWSA();
	bool unuseWSA();
	//�I�������o�^
	void reserveOnEnd(VOIDPROC proc);
	void _doReserveationOnEnd();
	//���̑�
	void showVersion(Window *owner,const wchar *date=LTEXT(__DATE__),const wchar *time=LTEXT(__TIME__));
	void shellOpen(wchar *fname,Window *owner);
	//�^�X�N�A�C�R��
	int addTaskIcon(Icon *icon,wchar *tiptext,Window *callback);
	void deleteTaskIcon(int id);
};

class Library : public App{
public:
	Library(const wchar *dllname);
	virtual ~Library();
	int init();
};

#include "c_Icon.h"



extern App *app;

class Screen;
extern Screen *screen;

int call_wmain(int argc,const wchar *argv[]);
