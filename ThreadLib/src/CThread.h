#pragma once

#include "CEvent.h"
#include "CCriticalSection.h"
#include <windows.h>
#include "design/CNamable.h"
#include "CRawThread.h"
using namespace c2lib;

/*!
	スレッドクラス

	スレッドを実装する場合、このクラスを継承し、runを定義する。

	【生成】
	newしただけではスレッドは作成されない。
	Create()を呼んだ時点でスレッドが作成され、そのスレッドは即開始される。

	【破棄】
	Destroy()を呼ぶとスレッドは中断され破棄される。
	デストラクタ時にもDestroy()は呼ばれる。(スレッド実行中にデストラクタが呼ばれた場合、スレッドはその時点で中断する)

	スレッド動作が完了すると、状態は STATE_DONE となる。(自動deleteはされない)
*/
/*
	新仕様の考案
	STATE_NONE
	↓
	↓Create()
	↓
	STATE_RUNNING


	STATE_RUNNING
	↓
	↓Destroy() またはスレッド完了
	↓
	STATE_NONE

	【イベント】
	OnThreadBegin
	OnThreadEnd
*/
class CThread : public CRawThread, public CNameHolder{
public:
	// -- -- 型 -- -- //
	//状態
	enum EState{
		STATE_NONE,		//生成されていない
		STATE_RUNNING,		//動作中
		STATE_SUSPENDED,	//一時停止中
		STATE_DONE,			//動作終了済
	};
protected:
	//static
	static DWORD WINAPI S_EntryPoint(LPVOID lpParameter);

public:
	//コンストラクタ・デストラクタ
	CThread();
	virtual ~CThread();
	void _Clear();

	//生成
	void Create();					// 使う人が呼びましょう //

	//状態ラップ
	bool IsRunning() const{ return m_hThread != NULL; }

protected:
	//動作
	virtual DWORD Run()=0;			// ※注：スレッド内で呼ばれる //

	//イベント
	virtual void OnThreadBegin(){}	// ※注：スレッド内で呼ばれる //
	virtual void OnThreadEnd(){}	// ※注：スレッド内で呼ばれる //

private:
	CThread**	m_ppcThread;
};
