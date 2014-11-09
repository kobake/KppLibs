#pragma once

#include <vector>

class CThreadInfo{
public:
	CThreadInfo(HANDLE hProcessRef, DWORD dwThreadID);
	void Print();

	//���
	HANDLE GetProcessHandle() const;
	DWORD GetThreadID() const;

	//�q
	std::vector<HWND> GetWindowList() const;

private:
	HANDLE	m_hProcessRef;
	DWORD	m_dwThreadID;
};
