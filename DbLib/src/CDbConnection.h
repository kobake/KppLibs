#pragma once

#include "_required.h"
#include "../../ThreadLib/ThreadLib.h" //CCriticalSection

class CDbRecord;

/*
	使い方
		g_db->OpenMdb(L"nichan.mdb");
*/
class CDbConnection{
public:
	//初期化と終了処理
	CDbConnection();
	~CDbConnection();
	//接続
	void Open(const wchar_t* szConString);
	void OpenMdb(const wchar_t* szMdbPath);		//!< .mdbを開く
	void Close();
	//操作
	void Query(CDbRecord* pcDbRecord, const wchar_t* szSql, ...);
	void QueryF(CDbRecord* pcDbRecord, const wchar_t* szSql, ...);
	void QuerySafe(CDbRecord* pcDbRecord, const wchar_t* szSql, ...); //####################実装
	void Execute(const wchar_t* szSql);						//例:「insert into A(a,b) values('abc','def')」
	void ExecuteF(const wchar_t* szSql, ...);				//例:「insert into A(a,b) values('%ls','%ls')」「I''m a student.」「This is an ''apple.''」
	void ExecuteSafe(const wchar_t* szSqlTemplate, ...);	//例:「insert into A(a,b) values('?','?')」「I'm a student.」「This is an 'apple.'」
	//実装アクセス
	_ConnectionPtr _GetPtr() const;
private:
	_ConnectionPtr		m_pCn;
	CCriticalSection	m_cCriticalSection;
};
