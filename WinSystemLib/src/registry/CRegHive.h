#pragma once

class CRegHive{
public:
	//コンストラクタ・デストラクタ
	CRegHive(const wchar_t* root_name); //※"\\"が含まれる場合は、それ以降を無視します
	CRegHive(HKEY _hRootKey);
	CRegHive();
	CRegHive(const CRegHive& rhs) : m_hKey(rhs.m_hKey) { } //コピー可
	CRegHive& operator = (const CRegHive& rhs){ m_hKey=rhs.m_hKey; return *this; } //コピー可
	//動作
	HKEY getHandle() const{ return m_hKey; }

	//情報
	const wchar_t* ToString() const;
private:
	HKEY m_hKey;
};
