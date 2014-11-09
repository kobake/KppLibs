#pragma once

#include <windows.h> //HKEY
#include <string>

class CRegHive;
class CRegValue;

class CRegKey{
private:
	//コピー不可
	CRegKey(const CRegKey&);
	CRegKey& operator = (const CRegKey&);

public:
	//コンストラクタ・デストラクタ
	CRegKey(const CRegHive& hive,const wchar_t* key_name,bool to_create);
	CRegKey(const wchar_t* szKeyName);
	CRegKey(const CRegKey& cParentKey, const wchar_t* szChildKey);
	CRegKey();
	virtual ~CRegKey();

	//状態
	bool IsValid() const{ return m_hKey != NULL; }

	//動作
	void Open(const CRegHive& hive,const wchar_t* key_name,bool to_create);
	void Open(const wchar_t* szKeyName);
	void Open(const CRegKey& cParentKey, const wchar_t* szChildKey);
	void Close();

	//値操作
	void SetValue(const wchar_t* name,const CRegValue& value);
	CRegValue GetValue(const wchar_t* name);
	CRegValue TryGetValue(const wchar_t* name);

	//列挙
	std::wstring EnumKey(int nIndex) const;
	bool EnumValue(int nIndex, std::wstring* pstrNameBuf) const;

	//直アクセス
	HKEY _GetHKEY() const{ return m_hKey; }

	//情報
	const wchar_t* ToString() const{ return m_strSummary.c_str(); }
private:
	std::wstring	m_strSummary;
	HKEY			m_hKey;
};

