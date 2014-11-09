#include "common/_required.h"
#include "CTimer.h"

//タイマーIDとTimer*の関連付け
#include <map>
using namespace std;
static map<UINT_PTR, CTimer*> g_mapTimers;

//コールバック
static VOID CALLBACK TimerProc(HWND hwnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
{
	if(g_mapTimers.find(idEvent)!=g_mapTimers.end()){
		g_mapTimers[idEvent]->OnTimer();
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       初期化と終了                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CTimer::CTimer()
: m_nTimerId(0)
, m_nIntervalMs(0)
{
}

CTimer::~CTimer()
{
	SetInterval(0); //タイマを殺す
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           設定                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//nIntervalMsに0を指定するとタイマー停止
void CTimer::SetInterval(int nIntervalMs)
{
	m_nIntervalMs = nIntervalMs;
	if(m_nIntervalMs>0){
		//タイマ作成
		m_nTimerId = ::SetTimer(NULL, NULL, m_nIntervalMs, TimerProc);
		//インスタンス記録
		g_mapTimers[m_nTimerId] = this;
	}
	else{
		if(m_nTimerId!=0){
			//インスタンス記録解除
			g_mapTimers.erase(m_nTimerId);
			//タイマ破棄
			::KillTimer(NULL, m_nTimerId);
			m_nTimerId = 0;
		}
	}
}

