#pragma once

class CObserver;
class CSubject;
#include <vector>

class CSubject{
public:
	virtual ~CSubject();
	void AttachObserver(CObserver* pcObserver);
	void DetachObserver(CObserver* pcObserver);
	void NotifyToObservers(int nNotifyCode = 0, int nNotifyParam = 0);
protected:
	int _findObserver(CObserver* pcObserver);
private:
//	std::vector<CObserver*>	m_vObservers;
};

class CObserver{
public:
	virtual ~CObserver();
	virtual void OnSubjectUpdate(int nCode, int nParam, CSubject* pcFrom){}
};
