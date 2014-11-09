#include "common/_required.h"
#include "CThread2.h"

void CThread2Listener::_Listen(CThread2* pcThread)
{
	assert(m_pcThreadRef==NULL);
	m_pcThreadRef = pcThread;
	m_pcThreadRef->m_vListeners.push_back(this);
}
