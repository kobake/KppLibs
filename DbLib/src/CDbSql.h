#pragma once

#include <string>
#include <vector>

class CDbSql{
public:
	//SQL文構築
	void SetTemplate(const wchar_t* szTemplate);
	int GetParameterCount() const{ return (int)m_vParameters.size(); }
	void SetParameters(const wchar_t* param1, ...);
	void SetParameter(int nIndex, const wchar_t* param);
	//SQL文取得
	const wchar_t* GetSql() const;
private:
	std::wstring				m_strTemplate;
	std::vector<std::wstring>	m_vParameters;

	//バッファ
	mutable std::wstring m_bufSql;
};
