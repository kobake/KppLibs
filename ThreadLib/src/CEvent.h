#pragma once

#include <windows.h>
#include <assert.h>
#include <string>

namespace c2lib{

class CEvent{
public:
	/*!
		�C�x���g���쐬�B
	*/
	CEvent(
		LPCWSTR szName = NULL	//!< ���O�BNULL���w�肷��ƁA���O�����C�x���g���쐬�����B
	);
	explicit CEvent(HANDLE hEvent);	//�����̃C�x���g�����b�v����ꍇ
	~CEvent();
	const wchar_t* GetEventName() const	{ return m_strEventName.c_str(); }

	//!�j��
	void Dispose();

	//!�V�O�i����Ԃɂ���
	void Set()
	{
		assert(m_hEvent!=NULL);
		::SetEvent(m_hEvent);
	}

	//!��V�O�i����Ԃɂ���
	void Reset()
	{
		assert(m_hEvent!=NULL);
		::ResetEvent(m_hEvent);
	}

	//!�V�O�i����ԂɂȂ�܂ő҂�
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
