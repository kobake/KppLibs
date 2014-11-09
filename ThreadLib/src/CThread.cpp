#include "common/_required.h"
#include "CThread.h"
#include <LogLib.h>
#include <objbase.h> // CoInitialize

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CThread::CThread()
{
	// スレッド本体が参照しているクラスインスタンスへのポインタ //
	m_ppcThread = NULL;
}

CThread::~CThread()
{
	// スレッド本体が参照しているクラスインスタンスへのポインタ //
	if(m_ppcThread){
		*m_ppcThread = NULL;
	}

	//スレッドがまだ生きている場合は、終了するまで待つ？
	if(m_hThread){
		KLOG(_FATAL, "[~CThread] Error: thread [%ls] is still running.", this->GetName());
#if 0
		while(1){
			bool bResult = WaitForThreadEnd(500);
			if(!bResult){
				break;
//				assert(0);
			}
		}
#endif
	}
	_Clear();
}

void CThread::_Clear()
{
	//スレッドハンドルを解放する
	if(m_hThread){
		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_ppcThread = NULL;
	}
	//その他のメンバ変数をクリアする
	//・特になし
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        生成と破棄                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//生成
void CThread::Create()
{
	CRITICAL_ENTER(m_cCritical);

	//既に存在していたらエラー
	if(m_hThread){ assert(0); return; }

	//スレッド作成
	DWORD dwThreadId = 0;
	m_hThread = CreateThread(
		NULL,				//セキュリティ記述子。NULLを指定すると、取得したハンドルを継承できない。
		0,					//スタックサイズ。0を指定すると、呼び出し側と同じか、既定のサイズが割り当てられる。
		S_EntryPoint,		//エントリーポイント
		this,				//スレッドの引数
		CREATE_SUSPENDED,	//作成オプション。とりあえず即時開始はしない
		&dwThreadId			//[out] スレッドID
	);
	assert(m_hThread);

	//スレッド開始
	::ResumeThread(m_hThread);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           動作                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

DWORD WINAPI CThread::S_EntryPoint(LPVOID lpParameter)
{
	CThread* pcThread=reinterpret_cast<CThread*>(lpParameter);

	// スレッド本体が参照しているクラスインスタンスへのポインタ //
	pcThread->m_ppcThread = &pcThread;

	//初期化処理
	HRESULT ret=CoInitialize(NULL);
	assert(ret==S_OK);
	if(pcThread){
		pcThread->OnThreadBegin();
	}

	//処理
	DWORD dwExitCode = 0;
	if(pcThread){
		dwExitCode = pcThread->Run();
	}

	//処理終了
	if(pcThread){
		pcThread->OnThreadEnd();
		pcThread->_Clear();
	}
	CoUninitialize();

	return dwExitCode;
}

