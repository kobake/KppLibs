#pragma once

#include <vector>
class CModule;
class CProcessImp;
class CThreadInfo;

class CProcess{
public:
	//コンストラクタ・デストラクタ
	CProcess(DWORD dwProcessID=0);
	CProcess(const CProcess& rhs);
	~CProcess();

	//自分
	bool IsValid() const;
	DWORD GetProcessID() const;
	HANDLE GetProcessHandle() const;
	void Print();

	//子モジュール
	CModule GetMainModule();
	std::vector<CModule> GetModuleList() const;

	//スレッド
	std::vector<CThreadInfo> GetThreadList() const;
private:
	CProcessImp*	m_pImp;
};
