#include "common/_required.h"
#include "ThreadUtil.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       スレッド関数                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//########### CRT に影響を与えるため、CreateThreadは使うべきではない。
//########### 代わりに _beginthreadex を使うこと。

HANDLE c2lib::Thread_Begin(
	Thread_EntryPoint	lpEntryPoint,
	LPVOID				lpParam,
	const CEvent*		pcInitDoneEvent
)
{
	//スレッド作成
	DWORD dwThreadId = 0;
	HANDLE hThread = CreateThread(
		NULL,			//セキュリティ記述子。NULLを指定すると、取得したハンドルを継承できない。
		0,				//スタックサイズ。0を指定すると、呼び出し側と同じか、既定のサイズが割り当てられる。
		lpEntryPoint,	//エントリーポイント
		lpParam,		//スレッドの引数
		0,				//作成オプション
		&dwThreadId		//[out] スレッドID
	);
	assert(hThread!=NULL);

	//スレッド初期処理が完了するまで待つ
	if(pcInitDoneEvent){
		pcInitDoneEvent->WaitForSignal();
	}

	return hThread;
}
