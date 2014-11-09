#include "_required.h"
#include "CDbConnection.h"
#include "CDbRecord.h"
#include <string>
#include <vector>
#include "util/cpp_swprintf.h"
#include "CDbSql.h"
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       初期化と終了                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CDbConnection::CDbConnection()
: m_pCn(NULL)
{
	try{
		TESTHR(m_pCn.CreateInstance(__uuidof(Connection)));
	}
	catch(...){
		debugPrint(L"CdbConnectionエラー\n");
		throw;
	}
}

CDbConnection::~CDbConnection()
{
	m_pCn.Release();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           接続                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDbConnection::Open(const wchar_t* szConString)
{
	CRITICAL_ENTER(m_cCriticalSection);

//	debugPrint(L"接続文字列は[%ls]です\n", szConString );
	m_pCn->Open(szConString, "", "", adConnectUnspecified);
}

//! .mdbを開く
void CDbConnection::OpenMdb(const wchar_t* szMdbPath)
{
	CRITICAL_ENTER(m_cCriticalSection);

	wstring s = (wstring)L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + szMdbPath;
	this->Open(s.c_str());
}

void CDbConnection::Close()
{
	CRITICAL_ENTER(m_cCriticalSection);

	m_pCn->Close();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           操作                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDbConnection::Query(CDbRecord* pcDbRecord, const wchar_t* szSql, ...)
{
	CRITICAL_ENTER(m_cCriticalSection);

	//整形
	wchar_t buf[512];
	va_list v;
	va_start(v, szSql);
	int n = vswprintf(buf, _countof(buf), szSql, v);
	va_end(v);
	buf[n] = 0;

	//送信
	pcDbRecord->_Query(buf, m_pCn);
}

void CDbConnection::Execute(const wchar_t* szSql)
{
	CRITICAL_ENTER(m_cCriticalSection);

	//送信
	variant_t vRecordsAffected(0L);
	m_pCn->BeginTrans();
	try{
		m_pCn->Execute(szSql, &vRecordsAffected, adOptionUnspecified);
	}
	catch(const _com_error& e){
		wstring s = (wstring)L"SQL = " + szSql;
		wprintf(L"SQL error: %ls\n", szSql);
		throw CComError(e, s.c_str());
	}
	HRESULT hr = m_pCn->CommitTrans();
	TESTHR(hr);
}

void CDbConnection::ExecuteF(const wchar_t* szSql, ...)
{
	//整形
	wstring buf;
	va_list v;
	va_start(v,szSql);
	buf = cpp_vswprintf(szSql, v);
	va_end(v);

	//送信
	Execute(buf.c_str());
}

void CDbConnection::ExecuteSafe(const wchar_t* szSqlTemplate, ...)
{
	//SQL構築
	CDbSql cSql;
	cSql.SetTemplate(szSqlTemplate);
	va_list v;
	va_start(v,szSqlTemplate);
	int n = cSql.GetParameterCount();
	for(int i=0;i<n;i++){
		cSql.SetParameter(i, va_arg(v, const wchar_t*));
	}
	va_end(v);

	//送信
	Execute(cSql.GetSql());
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       実装アクセス                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

_ConnectionPtr CDbConnection::_GetPtr() const
{
	return m_pCn;
}
