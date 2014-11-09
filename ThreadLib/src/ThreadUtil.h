#pragma once

#include "CEvent.h"

//スレッド関数
namespace c2lib{

typedef DWORD (WINAPI *Thread_EntryPoint)(LPVOID);

HANDLE Thread_Begin(
	Thread_EntryPoint	lpEntryPoint,			//!< エントリポイント
	LPVOID				lpParam,				//!< ユーザ定義値
	const CEvent*		pcInitDoneEvent	= NULL	//!< スレッド初期処理完了イベント (同期が必要なら渡す)
);

}//namespace c2lib
