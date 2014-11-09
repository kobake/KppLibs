#pragma once

#include <winsock2.h> //windows.h���O�ɂȂ��ƃ_��
#include <windows.h> //CRITICAL_SECTION

namespace c2lib{

//!�N���e�B�J���Z�N�V����
class CCriticalSection{
public:
	CCriticalSection(bool bInit = true)
	: m_bInited(false)
	{
		if(bInit){
			_Init();
		}
	}
	~CCriticalSection()
	{
		if(m_bInited){
			::DeleteCriticalSection(&m_sCriticalSection);
			m_bInited = false;
		}
	}
protected:
	//������
	void _Init()
	{
		::InitializeCriticalSection(&m_sCriticalSection);
		m_bInited = true;
	}
public:
	//�g�p
	bool IsInited() const
	{
		return m_bInited;
	}
	void Enter()
	{
		::EnterCriticalSection(&m_sCriticalSection);
	}
	void Leave()
	{
		::LeaveCriticalSection(&m_sCriticalSection);
	}
private:
	CRITICAL_SECTION	m_sCriticalSection;
	bool				m_bInited;
};

//!�N���e�B�J���Z�N�V�����˓��w���p�N���X
class CCriticalSectionEnter{
public:
	CCriticalSectionEnter(CCriticalSection* pcCriticalSection)
	: m_pcCriticalSectionRef(pcCriticalSection)
	{
		m_pcCriticalSectionRef->Enter();
	}
	~CCriticalSectionEnter()
	{
		m_pcCriticalSectionRef->Leave();
	}
private:
	CCriticalSection* m_pcCriticalSectionRef;
};

//!�N���e�B�J���Z�N�V�����˓��}�N��
#define CRITICAL_ENTER(c) CCriticalSectionEnter _cCriticalSectionEnter##__LINE__##_(&c)
#define CRITICAL(c) CCriticalSectionEnter _cCriticalSectionEnter##__LINE__##_(&c)

//! �֐�����static�錾���Ă����S�ȃN���e�B�J���Z�N�V����
class CStaticCriticalSection : public CCriticalSection{
protected:
	static CCriticalSection gm_init;
public:
	CStaticCriticalSection()
	: CCriticalSection(false)
	{
		CRITICAL_ENTER(gm_init);

		if(!IsInited()){
			_Init();
		}
	}
};



} //namespace c2lib

using namespace c2lib;
