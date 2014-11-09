#pragma once

#include <string>
#include <vector>

class CDbSql{
public:
	//SQL���\�z
	void SetTemplate(const wchar_t* szTemplate);
	int GetParameterCount() const{ return (int)m_vParameters.size(); }
	void SetParameters(const wchar_t* param1, ...);
	void SetParameter(int nIndex, const wchar_t* param);
	//SQL���擾
	const wchar_t* GetSql() const;
private:
	std::wstring				m_strTemplate;
	std::vector<std::wstring>	m_vParameters;

	//�o�b�t�@
	mutable std::wstring m_bufSql;
};
