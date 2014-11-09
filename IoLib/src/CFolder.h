#pragma once

#include <vector>
#include <string>
#include <list>

class CFolder{
public:
	// �������ƏI�� //
	CFolder(const std::wstring& strPath) : m_strPath(strPath) { }
	// �擾 //
	const std::wstring&		GetFolderPath() const	{ return m_strPath; }
	// ���� //
	std::vector<std::wstring> EnumFilesPath(const wchar_t* szPattern);		//�t�@�C���݂̂��
	std::vector<std::wstring> EnumChildrenName(const wchar_t* szPattern);	//�t�@�C���ƃf�B���N�g�����
	void EnumItems(const wchar_t* szPattern, std::vector<WIN32_FIND_DATA>* pvFiles);
	void EnumItems(const wchar_t* szPattern, std::list<WIN32_FIND_DATA>* pvFiles);
	// �e�X�g //
	int Test();

private:
	std::wstring m_strPath;
};

