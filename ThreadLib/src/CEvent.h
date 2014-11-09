#pragma once

#include <windows.h>
#include <assert.h>
#include <string>

namespace c2lib{

class CEvent{
public:
	/*!
		イベントを作成。
	*/
	CEvent(
		LPCWSTR szName = NULL	//!< 名前。NULLを指定すると、名前無しイベントが作成される。
	);
	explicit CEvent(HANDLE hEvent);	//既存のイベントをラップする場合
	~CEvent();
	const wchar_t* GetEventName() const	{ return m_strEventName.c_str(); }

	//!破棄
	void Dispose();

	//!シグナル状態にする
	void Set()
	{
		assert(m_hEvent!=NULL);
		::SetEvent(m_hEvent);
	}

	//!非シグナル状態にする
	void Reset()
	{
		assert(m_hEvent!=NULL);
		::ResetEvent(m_hEvent);
	}

	//!シグナル状態になるまで待つ
	DWORD WaitForSignal(DWORD dwTimeout = INFINITE) const
	{
		assert(m_hEvent!=NULL);
		return ::WaitForSingleObject(m_hEvent, dwTimeout);
	}

private:
	std::wstring	m_strEventName;
	HANDLE			m_hEvent;
	bool			m_bCreated;
};



} //namespace c2lib


using namespace c2lib;
