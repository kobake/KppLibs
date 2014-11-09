/*
	newされると直ちに開始され、
	終了すると自動的にdeleteされるタイプの
	スレッドモジュール
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

		//Run実行
		int nRet = 0;
		try{
			nRet = pcThread->Run();
		}
		catch(const std::exception& e){
			printf("ThreadError: %hs\n", e.what());
		}
		
		//終了したら自動的にdelete
		delete pcThread;

		return nRet;
	}
public:
	CThread2()
	: m_dwThreadId(0)
	{
		HANDLE hThread = ::CreateThread(
			NULL,			//SECURITY_ATTRIBUTES,
			0,				//スタックサイズ
			_Entry,			//エントリポイント
			this,			//エントリポイントに渡すLPVOIDパラメータ
			0,				//フラグ
			&m_dwThreadId	//スレッドID
		);
		if(!hThread)throw std::exception("CreateThread failed");
	}
	virtual ~CThread2()
	{
		//死ぬ前にリスナに知らせる
		for(int i=0;i<(int)m_vListeners.size();i++){
			m_vListeners[i]->OnThreadDead();
		}
	}
protected:
	virtual int Run() = 0;
private:
	DWORD							m_dwThreadId;
	std::vector<CThread2Listener*>	m_vListeners; //####このコンテナはできればスレッドセーフにしておきたいところ
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
		//###本当はイベントを用いて待つのがスマート
		while(_GetThreadRef()){
			::Sleep(2);
		}
	}
	THREAD_CLASS* operator->(){ return static_cast<THREAD_CLASS*>(_GetThreadRef()); }
	const THREAD_CLASS* operator->() const{ return static_cast<const THREAD_CLASS*>(_GetThreadRef()); }
};
