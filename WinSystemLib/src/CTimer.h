#pragma once

class CTimer{
public:
	//�������ƏI��
	CTimer();
	virtual ~CTimer();
	//�ݒ�
	void SetInterval(int nMilliSec); //nMilliSec��0���w�肷��ƃ^�C�}�[��~
	int GetInterval() const{ return m_nIntervalMs; }
	//�C�x���g
	virtual void OnTimer(){}
private:
	UINT_PTR	m_nTimerId;
	int			m_nIntervalMs;
};
