#include "common/_required.h"
#include "CRawThread.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       ‰Šú‰»‚ÆI—¹                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CRawThread::~CRawThread()
{
	if(m_hThread){
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           “¯Šú                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool CRawThread::WaitForThreadEnd(int nTimeout) const
{
	if(m_hThread){
		DWORD dwTimeout = (nTimeout<0)?INFINITE:nTimeout;
		DWORD dwResult = ::WaitForSingleObject(m_hThread, dwTimeout);
		if(dwResult == WAIT_OBJECT_0){
			return true;
		}
		else if(dwResult == WAIT_TIMEOUT){
			return false;
		}
		else if(dwResult == WAIT_FAILED){ //“ä‚ÌƒGƒ‰[
			DWORD err = ::GetLastError();
			return false;
		}
		else{
			return false;
		}
	}
	else{
		//Š®—¹‚µ‚«‚Á‚Ä‚¢‚é‚Æ‚İ‚È‚·
		return true;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ‘€ì                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CRawThread::Suspend()
{
	CRITICAL_ENTER(m_cCritical);
	SuspendThread(m_hThread);
}

void CRawThread::Resume()
{
	CRITICAL_ENTER(m_cCritical);
	ResumeThread(m_hThread);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ‘®«                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//
void CRawThread::SetPriority(EPriority ePriority)
{
	CRITICAL_ENTER(m_cCritical);
	::SetThreadPriority(m_hThread, (int)ePriority);
}

CRawThread::EPriority CRawThread::GetPriority() const
{
	CRITICAL_ENTER(m_cCritical);
	return (EPriority)::GetThreadPriority(m_hThread);
}
