#pragma once

#include <vector>
#include <string>
#include "design/TSingleton.h"

/*
	�R�}���h���C�����������߂���B�i���s�t�@�C�����͏����j

	�R�}���h���C����:
		hoge.exe /hide /path=aabb /path2="D:\tmp\hoge" /path3 hello.cpp world.cpp

	�g�p��:
		bool b = CCommandLine::Instance()->GetFlagBool(L"/hide");				// true
		std::wstring s1 = CCommandLine::Instance()->GetFlagString(L"/path");	// aabb
		std::wstring s2 = CCommandLine::Instance()->GetFlagString(L"/path2");	// D:\tmp\hoge
		std::wstring s3 = CCommandLine::Instance()->GetFlagString(L"/path3");	// hello.cpp
		std::wstring s4 = CCommandLine::Instance()->GetValuedParam(0);			// world.cpp
*/
class CCommandLine : public TSingleton<CCommandLine>{
public:
	void						Initialize(const wchar_t* str);
	void						Initialize(int argc, wchar_t* argv[]);
	void						Initialize(const std::vector<std::wstring>& args);
	
	// �t���O //
	bool						GetFlagBool(const wchar_t* szKey) const;
	int							FindParam(const wchar_t* szKey) const;
	std::wstring				GetFlagString(const wchar_t* szKey) const;

	// C //
	int							argc() const;
	const wchar_t**				argv() const;

	// �l //
	std::wstring				GetParam(int iIndex) const;
	std::wstring				GetValuedParam(int iIndex) const;

	// �I�v�V�����ȊO�̂��ׂĂ�Ԃ� (���s�t�@�C�����͏���)
	const std::vector<std::wstring>&	GetValuedParams() const	{ return m_vValuedParameters; }
private:
	std::vector<std::wstring>	m_vParameters;			// �S�p�����[�^ //
	std::vector<std::wstring>	m_vValuedParameters;	// �I�v�V�����ȊO�̃p�����[�^ //
};
