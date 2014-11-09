#pragma once

class CTimer{
public:
	//初期化と終了
	CTimer();
	virtual ~CTimer();
	//設定
	void SetInterval(int nMilliSec); //nMilliSecに0を指定するとタイマー停止
	int GetInterval() const{ return m_nIntervalMs; }
	//イベント
	virtual void OnTimer(){}
private:
	UINT_PTR	m_nTimerId;
	int			m_nIntervalMs;
};
