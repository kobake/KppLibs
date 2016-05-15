#pragma once

#include <string>

class CNamable{
public:
	virtual ~CNamable(){ }
	virtual const wchar_t* GetName() const = 0;
};

class CNullNamable : public CNamable{
public:
	virtual const wchar_t* GetName() const{ return L""; }
};

class CNameHolder : public CNamable{
public:
	CNameHolder();
	CNameHolder(const wchar_t* szName);
	virtual const wchar_t* GetName() const{ return m_strName.c_str(); }
	void SetName(const wchar_t* szName){ m_strName = szName; }
private:
	std::wstring	m_strName;
};

