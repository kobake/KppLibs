#pragma once

#include <vector>
#include <string>
#include <list>

class CFolder{
public:
	// 初期化と終了 //
	CFolder(const std::wstring& strPath) : m_strPath(strPath) { }
	// 取得 //
	const std::wstring&		GetFolderPath() const	{ return m_strPath; }
	// 検索 //
	std::vector<std::wstring> EnumFilesPath(const wchar_t* szPattern);		//ファイルのみを列挙
	std::vector<std::wstring> EnumChildrenName(const wchar_t* szPattern);	//ファイルとディレクトリを列挙
	void EnumItems(const wchar_t* szPattern, std::vector<WIN32_FIND_DATA>* pvFiles);
	void EnumItems(const wchar_t* szPattern, std::list<WIN32_FIND_DATA>* pvFiles);
	// テスト //
	int Test();

private:
	std::wstring m_strPath;
};

