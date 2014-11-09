#pragma once

#include <shlobj.H>
#include <string>

class CVariant{
public:
	CVariant(const VARIANT& v) : m_v(v) { }
	~CVariant(){ Clear(); }
	std::wstring ToString() const;
	int Compare(const CVariant& rhs) const;
	void Clear()
	{
		::VariantClear(&m_v);
	}
	bool IsInt() const
	{
		switch(m_v.vt){
		case VT_I4:	
		case VT_UI1:
		case VT_I2:
			return true;
		default:
			return false;
		}
	}
	int GetInt() const
	{
		switch(m_v.vt){
		case VT_I4:		return m_v.lVal;
		case VT_UI1:	return m_v.bVal;
		case VT_I2:		return m_v.iVal;
		default:
			return 0;
		}
	}
private:
	VARIANT m_v;
};
