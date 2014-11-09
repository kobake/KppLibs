#include "common/_required.h"
#include "CTimer.h"

//�^�C�}�[ID��Timer*�̊֘A�t��
#include <map>
using namespace std;
static map<UINT_PTR, CTimer*> g_mapTimers;

//�R�[���o�b�N
static VOID CALLBACK TimerProc(HWND hwnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
{
	if(g_mapTimers.find(idEvent)!=g_mapTimers.end()){
		g_mapTimers[idEvent]->OnTimer();
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       �������ƏI��                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CTimer::CTimer()
: m_nTimerId(0)
, m_nIntervalMs(0)
{
}

CTimer::~CTimer()
{
	SetInterval(0); //�^�C�}���E��
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �ݒ�                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//nIntervalMs��0���w�肷��ƃ^�C�}�[��~
void CTimer::SetInterval(int nIntervalMs)
{
	m_nIntervalMs = nIntervalMs;
	if(m_nIntervalMs>0){
		//�^�C�}�쐬
		m_nTimerId = ::SetTimer(NULL, NULL, m_nIntervalMs, TimerProc);
		//�C���X�^���X�L�^
		g_mapTimers[m_nTimerId] = this;
	}
	else{
		if(m_nTimerId!=0){
			//�C���X�^���X�L�^����
			g_mapTimers.erase(m_nTimerId);
			//�^�C�}�j��
			::KillTimer(NULL, m_nTimerId);
			m_nTimerId = 0;
		}
	}
}

