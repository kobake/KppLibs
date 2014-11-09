#include "common/_required.h"
#include "CVariant.h"
#include <StringLib.h>
#include "util/std.h"

std::wstring CVariant::ToString() const
{
	switch(m_v.vt){
	case VT_BSTR:	return m_v.bstrVal;
	case VT_I4:		return tmp_itow(m_v.lVal);
	case VT_UI1:	return tmp_itow(m_v.bVal);
	case VT_I2:		return tmp_itow(m_v.iVal);
	case VT_R4:		return tmp_ftow(m_v.fltVal);
	case VT_R8:		return tmp_ftow(m_v.dblVal);
	case VT_BOOL:	return m_v.boolVal?L"TRUE":L"FALSE";
	/*
	case VT_ERROR:	n = m_v.scode
	case VT_CY:		n = m_v.cyVal
	case VT_DATE:	n = m_v.date
	case VT_BSTR:
	case VT_UNKNOWN:
	case VT_DISPATCH:
	case VT_ARRAY:
	*/
	}
	wchar_t buf[64];
	swprintf(buf,L"VARIANT(vt=%d)",m_v.vt);
	return buf;
}

int CVariant::Compare(const CVariant& rhs) const
{
	switch(m_v.vt){
	case VT_BSTR:	return wcscmp(m_v.bstrVal, rhs.m_v.bstrVal);
	case VT_I4:		return (int)m_v.lVal - (int)rhs.m_v.lVal;
	case VT_UI1:	return (int)m_v.bVal - (int)rhs.m_v.bVal;
	case VT_I2:		return (int)m_v.iVal - (int)rhs.m_v.iVal;
	case VT_R4:		return (int)unit<float>(m_v.fltVal - rhs.m_v.fltVal);
	case VT_R8:		return (int)unit<double>(m_v.dblVal - rhs.m_v.dblVal);
	case VT_BOOL:	return (int)m_v.boolVal - (int)rhs.m_v.boolVal;
	}
	return 0;
}
