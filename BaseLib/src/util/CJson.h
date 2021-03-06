#pragma once

#include <string>

class CJson{
public:
	// 初期化と終了 //
	CJson();
	~CJson();
	bool	LoadJsonFile(const wchar_t* szFilePath);

	// 取得 //
	std::wstring	GetString(const wchar_t* szKey) const;
	int				GetInt(const wchar_t* szKey) const;

private:
	void*	m_pTree;
};
