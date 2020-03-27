#include <BaseLib.h>
#include <BaseLibOld.h>
#include "../_old_app/c_App.h"
#include <StringLib.h>
#include "../_old_window/c_Screen.h"

#include <GraphicsLib.h>
#include <StringLib.h>
#include "src/CBitmap.h"

App* g_app;
Screen *screen;

App *AppCreate(HINSTANCE hInstance,HINSTANCE hPrevInstance);
int AppInit(int argc,const wchar *argv[],int nCmdShow);
int AppRoutine();
int AppError(int errcode);
int AppEnd();

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- --  コマンドライン引数の取得  -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //





// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- --  プログラム開始 WinMain/wmain  -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#include <BaseLib.h>

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    //日本語出力準備 
    setlocale(LC_ALL,"");

    int ret;
    CCommandLine cmdline;
    cmdline.Initialize(GetCommandLine());
    screen=new Screen();
    srand(GetTickCount());
    g_app=AppCreate(hInstance,hPrevInstance);
    ret=g_app->init();
    if(ret!=0){
        AppError(ret);
        goto end;
    }
    ret=AppInit(cmdline.argc(),cmdline.argv(),nCmdShow); if(ret!=0)goto end;
    ret=AppRoutine(); //g_app->windowsLoop();
end:
    AppEnd();
    g_app->_doReserveationOnEnd();
    delete g_app;
    delete screen;
    return ret;
}
int call_wmain(int argc,const wchar *argv[])
{
    //日本語出力準備 
    setlocale(LC_ALL,"Japanese");

    int ret;
    screen=new Screen();
    srand((unsigned int)time(NULL));
    g_app=AppCreate(NULL,NULL);
    ret=AppInit(argc,argv,0); if(ret!=0)goto end;
    ret=AppRoutine();
end:
    AppEnd();
    g_app->_doReserveationOnEnd();
    delete g_app;
    delete screen;
    return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//※_init_vを呼び出す前に、必ず is_dll, hInst, hPrevInst を初期化しておく
void App::_init_v()
{
    if(!is_dll){
        InitCommonControls();
    }

    //変数初期化
    softname=L"";
    usingWSA=0;
    hKeyHook=NULL;
    *error_msg=L'\0';
    memset(ni_list,0,sizeof(NOTIFYICONDATA*)*TASKICON_MAX);
    memset(task_icon,0,sizeof(HICON)*TASKICON_MAX);
    memset(&wsadata,0,sizeof(WSADATA));
    
    //EXE情報取得
    wchar *p;
    GetModuleFileName(hInst,exepath,sizeof(exepath)-1);
    wcscpy(exedir,exepath);
    p=wcschr(exedir,L'\\'); if(p!=NULL)*p=L'\0';

    //標準INIパス設定
    wchar inipath[sizeof(exepath)];
    wcscpy(inipath,exepath);
    p=wcsrchr(inipath,L'.'); if(p==NULL)p=wcschr(inipath,L'\0');
    wcscpy(p,L".ini");
    inifile=new CIniFile(inipath);

    //環境変数の設定
    if(!is_dll){
        setEnv(L"EXEDIR",exedir);
    }
    
}

App::App(const wchar *dllname)
: proc_on_end()
{
    is_dll=true;
    hInst=LoadLibrary(dllname);
    hPrevInst=NULL;
    //
    _init_v();
    //
    softname=L"";
    multi_run=false;
    hMutex=NULL;
}

App::App(HINSTANCE _hInst,HINSTANCE _hPrevInst,const wstring& _softname,const wchar *_mutex)
: proc_on_end()
{
    is_dll=false;
    hInst=_hInst;
    hPrevInst=_hPrevInst;
    //
    _init_v();
    //
    softname=_softname;
    //
    //ミューテックスの作成 (重複起動のチェック)
    multi_run=false;
    if(_mutex!=NULL){
        hMutex=CreateMutex(NULL,FALSE,_mutex);
        if(GetLastError()==ERROR_ALREADY_EXISTS){
            multi_run=true;
        }
    }else{
        hMutex=NULL;
    }
}
App::~App()
{
    for(int i=0;i<TASKICON_MAX;i++){
        deleteTaskIcon(i);
    }
    unuseWSA();
    if(hKeyHook!=NULL)unhookKeyboard();
    if(hMutex!=NULL){
        CloseHandle(hMutex);
    }
    delete inifile;
    //
    if(is_dll){
        FreeLibrary(hInst);
    }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- リソース  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Icon *App::loadIcon(int id,int w,int h)
{
    return new Icon(this,id,w,h);
}

CBitmap *App::loadBitmap(int id)
{
    return new CBitmap(this->getInstance(),id);
}

void *App::loadResource(int id,int type)
{
    HRSRC hResource=FindResource(hInst,MAKEINTRESOURCE(id),MAKEINTRESOURCE(type));
    if(hResource==NULL)return NULL;
    HGLOBAL hMemory=LoadResource(hInst,hResource);
    if(hMemory==NULL)return NULL;
    DWORD size = SizeofResource(hInst,hResource);
    void *ret=LockResource(hMemory);
    return ret;
}
void App::loadResource2(std::vector<BYTE>* buf, int id,int type)
{
    buf->clear();
    HRSRC hResource=FindResource(hInst,MAKEINTRESOURCE(id),MAKEINTRESOURCE(type));
    if(hResource==NULL)return;
    HGLOBAL hMemory=LoadResource(hInst,hResource);
    if(hMemory==NULL)return;
    DWORD size = SizeofResource(hInst,hResource);
    buf->resize(size);
    void *ret=LockResource(hMemory);
    BYTE* p = &(*buf)[0];
    memcpy(p, ret, size);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 環境・パス -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool App::multiRun()
{
    return multi_run;
}
void App::chDir(const wstring& path)
{
    _wchdir(path.c_str());
}
void App::setEnv(const wstring& name,const wstring& value)
{
    SetEnvironmentVariable(name.c_str(),value.c_str());
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- エラー処理  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void App::setErrorMessage(wchar *msg)
{
    wcssafecpy(error_msg,msg,sizeof(error_msg));
}
wchar *App::getErrorMessage()
{
    return error_msg;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- メッセージ処理  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int App::windowsLoop()
{
    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

int App::dialogLoop(Window *wnd,Accelerator *accel)
{
    return dialogLoop(wnd->getHWND(),accel);
}

int App::dialogLoop(HWND hwnd,Accelerator *accel)
{
    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        if(!(accel!=NULL && accel->translate(hwnd,&msg)) && !IsDialogMessage(hwnd,&msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

int App::acceleratorLoop(Window *wnd,Accelerator *accel)
{
    MSG msg;
    HWND hwnd=wnd->getHWND();
    while(GetMessage(&msg,NULL,0,0)){
        if(!(accel!=NULL && accel->translate(hwnd,&msg))){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

/*
    MSG msg;
    HACCEL hAccel=LoadAccelerators(hInstance,MAKEINTRESOURCE(ACC_MAIN));
    while(GetMessage(&msg,NULL,0,0)){
        if(!TranslateAccelerator(hwndMain,hAccel,&msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;

*/
int App::gameLoop(ROUTINE r)
{
    MSG msg; int ret;
    while(1){
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            if(msg.message==WM_QUIT){
                ret=msg.wParam;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }else{
            if(r()!=0){
                ret=0;
                break;
            }
        }
    }
    return ret;
}

void App::postQuit(int code)
{
    PostQuitMessage(0);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --  フック  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void App::hookKeyboard(HOOKPROC keyproc)
{
    hKeyHook=SetWindowsHookEx(WH_KEYBOARD,keyproc,hInst,0);
}

void App::unhookKeyboard()
{
    UnhookWindowsHookEx(hKeyHook);
    hKeyHook=NULL;
}

LRESULT App::callKeyboardNext(int code,WPARAM wParam,LPARAM lParam)
{
    return CallNextHookEx(hKeyHook,code,wParam,lParam);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- ネットワーク準備  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool App::useWSA()
{
    if(is_dll)return false;
    if(usingWSA==1){
        return true; //既に初期化済み
    }else{
        if(WSAStartup(MAKEWORD(1,1),&wsadata)==0){ // W32_32.DLL の初期化
            usingWSA=1;
            return true;
        }else{
            return false;
        }
    }
}
bool App::unuseWSA()
{
    if(is_dll)return false;
    if(usingWSA==0){
        return true; //既に解放済み
    }else{
        if(WSACleanup()==0){ // W32_32.DLL の解放
            usingWSA=0;
            return true;
        }else{
            return false;
        }
    }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  終了処理登録  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void App::reserveOnEnd(VOIDPROC proc)
{
    proc_on_end.put(-1,proc);
}

void App::_doReserveationOnEnd()
{
    while(proc_on_end.size()>0){
        proc_on_end.get(0)();
        proc_on_end.remove(0);
    }
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  その他  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void App::shellOpen(wchar *fname,Window *owner)
{
    ShellExecute((owner!=NULL)?owner->getHWND():NULL,L"open",fname,L"",exedir,SW_SHOWNORMAL);
}

void App::showVersion(Window *owner,const wchar *date,const wchar *time)
{
    wchar tmp[256];
    wcssafecpy(tmp,softname.c_str(),    sizeof(tmp));
    wcssafecat(tmp,L"\n",               sizeof(tmp));
    wcssafecat(tmp,L"Last update - ",   sizeof(tmp));
    wcssafecat(tmp,date,                sizeof(tmp));
    wcssafecat(tmp,L", ",               sizeof(tmp));
    wcssafecat(tmp,time,                sizeof(tmp));
    if(owner==NULL)owner=Window::getNullWindow();
    owner->messageBox(tmp,L"バージョン情報",MB_OK | MB_ICONINFORMATION);
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- タスクアイコン  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//IDを返す
int App::addTaskIcon(Icon *icon,wchar *tiptext,Window *callback)
{
    /*
    int id=-1;
    NOTIFYICONDATA *ni=NULL;
    for(int i=0;i<TASKICON_MAX;i++){
        if(ni_list[i]==NULL){
            id=i;
            ni_list[i]=(NOTIFYICONDATA*)calloc(1,sizeof(NOTIFYICONDATA));
            ni=ni_list[i];
            task_icon[i]=icon->clone();
            break;
        }
    }
    if(ni==NULL)return -1;

    ni->cbSize=sizeof(NOTIFYICONDATA);
    ni->hIcon=task_icon[i]->_getHICON();
    if(callback!=NULL)
        ni->hWnd=callback->getHWND();
    else
        ni->hWnd=NULL;
    ni->uCallbackMessage=WM_TASKTRAY;
    ni->uFlags=NIF_ICON | NIF_MESSAGE;
    ni->uID=id;
    if(tiptext!=NULL){
        strcpy(ni->szTip,tiptext);
        if(*tiptext!=L'\0'){
            ni->uFlags|=NIF_TIP;
        }
    }else{
        *ni->szTip=L'\0';
    }
    Shell_NotifyIcon(NIM_ADD,ni);
    return id;
    */
    return 0;
}

void App::deleteTaskIcon(int id)
{
    NOTIFYICONDATA *ni=NULL;
    if(id>=0 && id<TASKICON_MAX)
        ni=ni_list[id];
    if(ni==NULL)
        return;
    //
    Shell_NotifyIcon(NIM_DELETE,ni);
    m_free(ni_list[id]);
    m_delete(task_icon[id]);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- --  [class] Library  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Library::Library(const wchar *dllname)
: App(dllname)
{
}
Library::~Library()
{
}
int Library::init()
{
    return 0;
}
