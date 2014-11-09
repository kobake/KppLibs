#pragma once

#include <boost/shared_ptr.hpp>

class CDbRecord{
public:
	//初期化と終了
	CDbRecord();
	~CDbRecord();
	//取得
	int Count() const{ return m_pRs->RecordCount; }			//!< レコード数取得
	_bstr_t GetValue(const wchar_t* szColumnName) const;	//!< SQL結果取得
	void Next();											//!< 次のSQL結果へ進む
	bool Eof() const;										//!< SQL結果取得を終えたかどうか

public:
	//実装補助
	void _Query(const wchar_t* z, _ConnectionPtr pCn);	//!< SQL発行
	_RecordsetPtr _GetPtr() const{ return m_pRs; }		//!< 実装アクセス
private:
	_RecordsetPtr	m_pRs;
	bool			m_bOpened;
};
