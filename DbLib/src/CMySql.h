#pragma once

#include <vector>
#include <string>
#include "util/mywstring.h"

class CMySqlValue{
public:
	enum EType{
		TYPE_NONE,
		TYPE_STRING,
		TYPE_INT,
		TYPE_DOUBLE,
	};
public:
	CMySqlValue()								: m_eType(TYPE_NONE),	m_strValue(L""),				m_iValue(0),						m_dValue(0.0) { }
	CMySqlValue(const std::wstring& strValue)	: m_eType(TYPE_STRING),	m_strValue(strValue),			m_iValue(_wtoi(strValue.c_str())),	m_dValue(_wtof(strValue.c_str())) { }
	CMySqlValue(const wchar_t* szValue)			: m_eType(TYPE_STRING),	m_strValue(szValue),			m_iValue(_wtoi(szValue)),			m_dValue(_wtof(szValue)) { }
	CMySqlValue(int iValue)						: m_eType(TYPE_INT),	m_strValue(tmp_itow(iValue)),	m_iValue(iValue),					m_dValue(iValue) { }
	CMySqlValue(double dValue)					: m_eType(TYPE_DOUBLE),	m_strValue(tmp_ftow(dValue)),	m_iValue((int)dValue),				m_dValue(dValue) { }
	const std::string&	GetEscaped(void* pkMysql) const;
	bool		IsValid() const		{ return m_eType != TYPE_NONE; }
	bool		IsString() const	{ return m_eType == TYPE_STRING; }
	bool		IsInt() const		{ return m_eType == TYPE_INT; }
	bool		IsDouble() const	{ return m_eType == TYPE_DOUBLE; }
	int			GetLength() const	{ return (int)m_strValue.length(); }

private:
	EType			m_eType;
	std::wstring	m_strValue;
	int				m_iValue;
	double			m_dValue;
	mutable std::string		m_strEscaped;
};

class CMySql{
public:
	typedef std::vector<std::wstring>	CRow;
	typedef std::vector<CRow>			CRows;

public:
	CMySql();
	virtual ~CMySql();

	// 接続 //
	bool			Open( const char* host, const char* user, const char* pass, const char* dbname, int port = 3306 );	// DBコネクションを開きます。 //
	void			Close();							// DBコネクションを閉じます。 //
	void			Ping();

	// クエリ準備 //
	unsigned long	EscapeString( char *to, const char *from, unsigned long length );	// mysql_real_escape_stringのラッパー //

	// クエリ //
	int				RunQuery(	// クエリ実行。戻り値は処理したレコード件数。エラーの場合は -1。select時は 0。 //
		const char*	szQueryString,
		CMySqlValue	cValue0 = CMySqlValue(),
		CMySqlValue	cValue1 = CMySqlValue(),
		CMySqlValue	cValue2 = CMySqlValue(),
		CMySqlValue	cValue3 = CMySqlValue(),
		CMySqlValue	cValue4 = CMySqlValue(),
		CMySqlValue	cValue5 = CMySqlValue(),
		CMySqlValue	cValue6 = CMySqlValue(),
		CMySqlValue	cValue7 = CMySqlValue()
	);

	// クエリ後 //
	bool			_GetRows(CRows* pvRecords);	// SELECT結果取得 //
	const CRows&	GetRows() const			{ return m_cRows; }	// SELECT結果取得 //
	int				GetRecentInsertId();						// 直前のINSERT文のオートインクリメント型idを取得 //

	// エラー情報 //
	int				GetError() const		{ return m_err_no; }
	const char*		GetErrorMessage() const	{ return m_err_msg; }
	void			_SetError(int err_no, const char* msg = "", ...);

private:
	void*		m_pkMysql;
	int			m_err_no;			// エラー番号、0ならエラーなし//
	char		m_err_msg[1024*2];	// エラーメッセージ //
	CRows		m_cRows;

};

