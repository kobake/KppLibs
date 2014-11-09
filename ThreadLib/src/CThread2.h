/*
	new�����ƒ����ɊJ�n����A
	�I������Ǝ����I��delete�����^�C�v��
	�X���b�h���W���[��
*/
#pragma once

#include <windows.h>
#include <exception>
#include <stdio.h>


#include <vector>
#include <assert.h>
#include "design/CNamable.h"

class CThread2;
class CThread2Listener{
public:
	CThread2Listener()
	{
		m_pcThreadRef = NULL;
	}
	virtual ~CThread2Listener(){}
	void			OnThreadDead()			{ m_pcThreadRef = NULL; }
	CThread2*		_GetThreadRef()			{ return m_pcThreadRef; }
	const CThread2*	_GetThreadRef() const	{ return m_pcThreadRef; }
protected:
	void _Listen(CThread2* pcThread);
private:
	CThread2* m_pcThreadRef;
};

class CThread2 : public CNameHolder{
public:
	friend class CThread2Listener;
protected:
	static DWORD WINAPI _Entry(LPVOID lpParam)
	{
		CThread2* pcThread = (CThread2*)lpParam;

		//Run���s
		int nRet = 0;
		try{
			nRet = pcThread->Run();
		}
		catch(const std::exception& e){
			printf("ThreadError: %hs\n", e.what());
		}
		
		//�I�������玩���I��delete
		delete pcThread;

		return nRet;
	}
public:
	CThread2()
	: m_dwThreadId(0)
	{
		HANDLE hThread = ::CreateThread(
			NULL,			//SECURITY_ATTRIBUTES,
			0,				//�X�^�b�N�T�C�Y
			_Entry,			//�G���g���|�C���g
			this,			//�G���g���|�C���g�ɓn��LPVOID�p�����[�^
			0,				//�t���O
			&m_dwThreadId	//�X���b�hID
		);
		if(!hThread)throw std::exception("CreateThread failed");
	}
	virtual ~CThread2()
	{
		//���ʑO�Ƀ��X�i�ɒm�点��
		for(int i=0;i<(int)m_vListeners.size();i++){
			m_vListeners[i]->OnThreadDead();
		}
	}
protected:
	virtual int Run() = 0;
private:
	DWORD							m_dwThreadId;
	std::vector<CThread2Listener*>	m_vListeners; //####���̃R���e�i�͂ł���΃X���b�h�Z�[�t�ɂ��Ă��������Ƃ���
};


#include <assert.h>

class CThread2;
template <class THREAD_CLASS>
class CThread2Handle : public CThread2Listener{
public:
	CThread2Handle()
	{
	}
	CThread2Handle(THREAD_CLASS* pcThread)
	{
		_Listen(pcThread);
	}
	void operator = (THREAD_CLASS* pcThread)
	{
		_Listen(pcThread);
	}
	void WaitForDie()
	{
		//###�{���̓C�x���g��p���đ҂̂��X�}�[�g
		while(_GetThreadRef()){
			::Sleep(2);
		}
	}
	THREAD_CLASS* operator->(){ return static_cast<THREAD_CLASS*>(_GetThreadRef()); }
	const THREAD_CLASS* operator->() const{ return static_cast<const THREAD_CLASS*>(_GetThreadRef()); }
};
