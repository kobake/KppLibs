/*
	パイプを介して入出力を行うタイプの
	プロセス起動モジュール
*/
#pragma once

#include <windows.h>

/*
	プロセス起動
	CProcess2 process("hoge.exe hoge hoge", NULL, NULL, NULL);

	デストラクタはプロセスハンドルを解放するだけなので、プロセスを強制終了させるわけではない。
*/
class CProcess2{
public:
	CProcess2(
		const wchar_t*	szCommand,
		HANDLE			hStdOutput,
		HANDLE			hStdError,
		HANDLE			hStdInput
	);
	~CProcess2();

	// 状態 //
	bool	IsAlive() const
	{
		DWORD n = ::WaitForSingleObject(m_hProcess, 0);
		return n == WAIT_TIMEOUT;
	}

	// 生情報 //
	HANDLE _GetHandle() const{ return m_hProcess; }
private:
	HANDLE m_hProcess;
};

