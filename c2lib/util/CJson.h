#pragma once

#include <string>

class CJson{
public:
	// ‰Šú‰»‚ÆI—¹ //
	CJson();
	~CJson();
	bool	LoadJsonFile(const wchar_t* szFilePath);

	// æ“¾ //
	std::wstring	GetString(const wchar_t* szKey) const;
	int				GetInt(const wchar_t* szKey) const;

private:
	void*	m_pTree;
};
