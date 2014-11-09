#include "common/_required.h"
#include "CProcess2.h"

CProcess2::CProcess2(
	const wchar_t*	szCommand,
	HANDLE			hStdOutput,
	HANDLE			hStdError,
	HANDLE			hStdInput
)
{
	//プロセス作成
	PROCESS_INFORMATION pi = {0};
	{
		//スタートアップ情報
		STARTUPINFO si = {0};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hStdOutput;
		si.hStdError = hStdError;
		si.hStdInput = hStdInput;

		//コマンドライン
		wchar_t bufCommand[_MAX_PATH];
		wcscpy(bufCommand, szCommand); //###

		//作成
		BOOL bRet = ::CreateProcess(
			NULL,				//実行可能モジュールの名前
			bufCommand,			//コマンドライン文字列
			NULL,				//セキュリティ記述子 (プロセス)
			NULL,				//セキュリティ記述子 (スレッド)
			TRUE,				//ハンドルの継承オプション
			CREATE_NO_WINDOW,	//作成のフラグ (コンソールアプリケーションをコンソールウィンドウなしで実行させます。)
			NULL,				//新しい環境ブロック
			NULL,				//カレントディレクトリ
			&si,				//スタートアップ情報
			&pi					//プロセス情報
		);

		//必要なハンドル保持
		m_hProcess = pi.hProcess;

		//不要なハンドル解放
		::CloseHandle(pi.hThread);
	}
}

CProcess2::~CProcess2()
{
	::CloseHandle(m_hProcess);
}
