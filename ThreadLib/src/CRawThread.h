#pragma once

#include <windows.h>
#include "CEvent.h"
#include "CCriticalSection.h"

class CRawThread{
public:
	//プライオリティ
	enum EPriority{
		PRI_LOWEST	= THREAD_PRIORITY_LOWEST,
		PRI_LOW		= THREAD_PRIORITY_BELOW_NORMAL,
		PRI_NORMAL	= THREAD_PRIORITY_NORMAL,
		PRI_HIGH	= THREAD_PRIORITY_ABOVE_NORMAL,
		PRI_HIGHEST	= THREAD_PRIORITY_HIGHEST,
	};

public:
	//初期化と終了
	CRawThread() : m_hThread(NULL) { }
	~CRawThread();

	//同期。完了しきったらtrueを返す。タイムアウトの場合はfalse。スレッドが無い場合は完了しきっているとみなし、true。
	bool WaitForThreadEnd(int nTimeout = -1) const;

	//操作
	void Suspend();
	void Resume();

	//属性
	void SetPriority(EPriority ePriority);
	EPriority GetPriority() const;
protected:
	HANDLE		m_hThread;		//スレッドハンドル
	//クリティカルセクション
	mutable CCriticalSection	m_cCritical;
};
