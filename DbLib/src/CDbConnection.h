#pragma once

#include "_required.h"
#include "../../ThreadLib/ThreadLib.h" //CCriticalSection

class CDbRecord;

/*
	�g����
		g_db->OpenMdb(L"nichan.mdb");
*/
class CDbConnection{
public:
	//�������ƏI������
	CDbConnection();
	~CDbConnection();
	//�ڑ�
	void Open(const wchar_t* szConString);
	void OpenMdb(const wchar_t* szMdbPath);		//!< .mdb���J��
	void Close();
	//����
	void Query(CDbRecord* pcDbRecord, const wchar_t* szSql, ...);
	void QueryF(CDbRecord* pcDbRecord, const wchar_t* szSql, ...);
	void QuerySafe(CDbRecord* pcDbRecord, const wchar_t* szSql, ...); //####################����
	void Execute(const wchar_t* szSql);						//��:�uinsert into A(a,b) values('abc','def')�v
	void ExecuteF(const wchar_t* szSql, ...);				//��:�uinsert into A(a,b) values('%ls','%ls')�v�uI''m a student.�v�uThis is an ''apple.''�v
	void ExecuteSafe(const wchar_t* szSqlTemplate, ...);	//��:�uinsert into A(a,b) values('?','?')�v�uI'm a student.�v�uThis is an 'apple.'�v
	//�����A�N�Z�X
	_ConnectionPtr _GetPtr() const;
private:
	_ConnectionPtr		m_pCn;
	CCriticalSection	m_cCriticalSection;
};
