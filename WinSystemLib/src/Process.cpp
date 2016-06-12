#include "_required.h"
#include "Process.h"
#include <windows.h>
#include <StringLib.h>

#define MY_MAX mmax(4*1024,_MAX_PATH)

//!プロセスを作成
HANDLE ProcessSystem::createProcess(const wchar_t* _cmdline)
{
	//CreateProcessの第2引数にconst配列は渡せないので、ここで書換可能な配列を作り直す
	static wchar_t cmdline[MY_MAX];
	wcssafecpy(cmdline,_cmdline,countof(cmdline));

	//STARTUPINFO用意
	STARTUPINFO si;
	memset(&si,0,sizeof(si));
	si.cb = sizeof(si);

	//PROCESS_INFORMATION用意
	PROCESS_INFORMATION pi;
	memset(&pi,0,sizeof(pi));

	//プロセス作成
	BOOL ret=CreateProcess(
		NULL,    //実行可能モジュールの名前
		cmdline, //コマンドラインの文字列
		NULL,    //セキュリティ記述子
		NULL,    //セキュリティ記述子
		FALSE,   //ハンドルの継承オプション
		0,       //作成のフラグ
		NULL,    //新しい環境ブロック
		NULL,    //カレントディレクトリの名前
		&si,     //スタートアップ情報
		&pi      //プロセス情報
	);

	//スレッドのハンドルは使わないので、クローズしておく
	CloseHandle(pi.hThread);

	//プロセスのハンドルだけ返す
	return pi.hProcess;
}


