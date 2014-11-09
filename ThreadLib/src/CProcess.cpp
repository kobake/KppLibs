#include "common/_required.h"
#include <windows.h>
#include <psapi.h>
#include "CProcess.h"
#include "CModule.h"
#include "CThreadInfo.h"
using namespace std;



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       CProcessImp                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

class CProcessImp{
public:
	//初期化と終了
	CProcessImp(DWORD dwProcessID)
	: m_dwProcessID(dwProcessID)
	{
		m_hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE,
			m_dwProcessID
		);
		m_nRefCnt=1;
	}
	~CProcessImp()
	{
		CloseHandle(m_hProcess);
	}
	void Use()
	{
		m_nRefCnt++;
	}
	void Unuse()
	{
		if(--m_nRefCnt<=0)delete this;
	}
	//インターフェース
	DWORD GetProcessID() const{ return m_dwProcessID; }
	HANDLE GetProcessHandle() const{ return m_hProcess; }
private:
	DWORD	m_dwProcessID;	//!< プロセスID
	HANDLE	m_hProcess;		//!< プロセスハンドル
	int		m_nRefCnt;		//!< 参照カウンタ
};


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CProcess::CProcess(DWORD dwProcessID)
{
	m_pImp = new CProcessImp(dwProcessID);
}

CProcess::CProcess(const CProcess& rhs)
{
	m_pImp = rhs.m_pImp;
	m_pImp->Use();
}

CProcess::~CProcess()
{
	m_pImp->Unuse();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

DWORD CProcess::GetProcessID() const
{
	return m_pImp->GetProcessID();
}

HANDLE CProcess::GetProcessHandle() const
{
	return m_pImp->GetProcessHandle();
}

void CProcess::Print()
{
	//モジュール情報
	vector<CModule> vModules=GetModuleList();
	for(int i=0;i<(int)vModules.size();i++){
		vModules[i].Print();
	}
	//スレッド情報
	vector<CThreadInfo> vThreads=GetThreadList();
	for(int i=0;i<(int)vThreads.size();i++){
		vThreads[i].Print();
	}
}

//取得
CModule CProcess::GetMainModule()
{
	HMODULE hModules[1];
	DWORD dw;
	BOOL bRet=EnumProcessModules(GetProcessHandle(),hModules,sizeof(hModules),&dw);
	int nModules=dw/sizeof(HMODULE);
	if(nModules>=1){
		return CModule(GetProcessHandle(),hModules[0]);
	}
	else{
		return CModule(0,0);
	}
}

vector<CModule> CProcess::GetModuleList() const
{
	//プロセス内のモジュールハンドルを取得
	HMODULE hModules[1024];
	DWORD dw;
	BOOL bRet=EnumProcessModules(GetProcessHandle(),hModules,sizeof(hModules),&dw);
	int nModules=dw/sizeof(HMODULE);

	//CModuleに変換
	vector<CModule> vModules;
	for(int i=0;i<nModules;i++){
		vModules.push_back(CModule(GetProcessHandle(),hModules[i]));
	}
	return vModules;
}

bool CProcess::IsValid() const
{
	return GetProcessHandle()!=NULL;
}

#include <tlhelp32.h>

vector<CThreadInfo> CProcess::GetThreadList() const
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if(hSnapshot==NULL)return vector<CThreadInfo>();

	vector<CThreadInfo> vThreads;
	THREADENTRY32 te32;
	te32.dwSize=sizeof(te32);
	if(Thread32First(hSnapshot,&te32)){
		do{
			if(te32.th32OwnerProcessID==this->GetProcessID()){
				vThreads.push_back(CThreadInfo(this->GetProcessHandle(),te32.th32ThreadID));
			}
		}while(Thread32Next(hSnapshot,&te32));
	}
	return vThreads;
}
