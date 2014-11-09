#pragma once

#include <vector>
class CModule;
class CProcessImp;
class CThreadInfo;

class CProcess{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CProcess(DWORD dwProcessID=0);
	CProcess(const CProcess& rhs);
	~CProcess();

	//����
	bool IsValid() const;
	DWORD GetProcessID() const;
	HANDLE GetProcessHandle() const;
	void Print();

	//�q���W���[��
	CModule GetMainModule();
	std::vector<CModule> GetModuleList() const;

	//�X���b�h
	std::vector<CThreadInfo> GetThreadList() const;
private:
	CProcessImp*	m_pImp;
};
