#pragma once

#include <string>
#include <map>

class CConfFile{
public:
	CConfFile();
	CConfFile(const wchar_t* szConfFile);
	operator bool() const;
	// ì«Ç›èëÇ´
	void			Load(const wchar_t* szConfFile);
	void			Save(const wchar_t* szConfFile) const;
	// éÊìæ
	std::wstring	GetString(const wchar_t* szKey) const;
	int				GetInt(const wchar_t* szKey, int defaultValue = 0) const;
	int				GetHexInt(const wchar_t* szKey, int defaultValue = 0) const;
	bool			Exists(const wchar_t* szKey) const;
	// ê›íË
	void			SetValue(const wchar_t* szKey, const wchar_t* szValue);
private:
	std::map<std::wstring, std::wstring> m_values;
};
