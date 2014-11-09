#pragma once

#include <string>

class CJson{
public:
	// �������ƏI�� //
	CJson();
	~CJson();
	bool	LoadJsonFile(const wchar_t* szFilePath);

	// �擾 //
	std::wstring	GetString(const wchar_t* szKey) const;
	int				GetInt(const wchar_t* szKey) const;

private:
	void*	m_pTree;
};
