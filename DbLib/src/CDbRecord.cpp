#include "_required.h"
#include "CDbRecord.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       ‰Šú‰»‚ÆI—¹                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CDbRecord::CDbRecord()
: m_pRs(NULL)
, m_bOpened(false)
{
	m_pRs = NULL;
	TESTHR_F(m_pRs.CreateInstance(__uuidof(Recordset)));
}

CDbRecord::~CDbRecord()
{
	m_pRs.Release();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ‘€ì                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

_bstr_t CDbRecord::GetValue(const wchar_t* szColumnName) const
{
	_variant_t V1;
	_variant_t V2;
	V1 = szColumnName;
	V2 = m_pRs->Fields->GetItem(szColumnName)->Value;
	if ( V2.vt == VT_NULL ) {
		return L"";
	}
	else {
		return V2;
	}
}

void CDbRecord::Next()
{
	m_pRs->MoveNext();
}

bool CDbRecord::Eof() const
{
	return m_pRs->EndOfFile!=0;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         ŽÀ‘••â•                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDbRecord::_Query(const wchar_t* z, _ConnectionPtr pCn)
{
	if(m_bOpened){
		m_pRs->Close();
	}
	m_pRs->Open(
		z,
		_variant_t((IDispatch *)pCn,true),
		adOpenKeyset,
		adLockOptimistic,
		adCmdText
	);
	m_bOpened = true;
}
