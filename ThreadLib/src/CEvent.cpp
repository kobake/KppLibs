#include "common/_required.h"
#include "CEvent.h"

CEvent::CEvent(
	LPCWSTR szName	//!< 名前。NULLを指定すると、名前無しイベントが作成される。
)
{
	m_strEventName = szName?szName:L"";
	m_hEvent = ::CreateEvent(
		NULL,	//セキュリティ記述子。NULLを指定すると、子プロセスはそのオブジェクトのハンドルを継承できない。
		TRUE,	//TRUE…手動リセット FALSE…自動リセット (WaitForSingleObjectを抜けると自動で非シグナル状態に戻る)
		FALSE,	//FALSE…初期状態＝非シグナル状態
		szName	//名前
	);
	m_bCreated = true;
}

CEvent::CEvent(HANDLE hEvent)	//既存のイベントをラップする場合
{
	m_hEvent = hEvent;
	m_bCreated = false;
}

CEvent::~CEvent()
{
	if(m_bCreated && m_hEvent!=NULL){
		CloseHandle(m_hEvent);
	}
}

//!破棄
void CEvent::Dispose()
{
	assert(m_hEvent!=NULL);
	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}
