#include "CMySql.h"
//// my_global より先にstl をインクルードしないとリンクエラーになる？
#include <my_global.h>	// bool 使えなくなります。
#include <mysql.h>
#ifdef bool
	#undef bool
#endif
#include "util/mywstring.h"
#include "util/CLog.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ライブラリ
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
static int MysqlEnd();

static int MysqlInit()
{
	static bool bFirst = true;
	if(bFirst){
		mysql_library_init(__argc, __argv, NULL);
		onexit(MysqlEnd);
		bFirst = false;
	}
	return 0;
}

static int MysqlEnd()
{
	mysql_library_end();
	return 0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 値
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
const std::string& CMySqlValue::GetEscaped(void* pkMysql) const
{
	if(m_strEscaped.length() == 0){
		std::string strUtf8 = cpp_wcstombs(m_strValue, CP_UTF8);
		std::vector<char> vBuf(strUtf8.length() * 2 + 1);
		unsigned long ret = mysql_real_escape_string( (MYSQL*)pkMysql, &vBuf[0], strUtf8.c_str(), (unsigned long)strUtf8.length() );
		vBuf[ret] = 0;
		m_strEscaped = &vBuf[0];
	}
	return m_strEscaped;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CMySql::CMySql()
{
	MysqlInit();
	m_pkMysql = 0;
	m_err_no = 0;			// エラー番号、0ならエラーなし//
	strcpy( m_err_msg, "" );
}

CMySql::~CMySql()
{
	Close();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 接続
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// DBコネクションを開きます。 //
bool CMySql::Open( const char* host, const char* user, const char* pass, const char* dbname, int port )
{
	// port指定が無ければデフォルトを使う //
	if(port <= 0){
		port = 3306;
	}

	_SetError(0);	// エラーをクリア //
	Close();	// 念のため前の接続を閉じます。

	m_pkMysql = mysql_init( NULL );
	if( !m_pkMysql ){
		_SetError( -1, "mysql_init() returned null" );
		Close();
		return false;
	}

	// MySQLサーバへ接続
	if( !mysql_real_connect( (MYSQL*)m_pkMysql, host, user, pass, dbname, port, NULL, 0 ) ){
		_SetError( -1, "mysql_real_connect() returned null(%s),host,%s,user,%s,pass,%s,dbname,%s"
			, mysql_error((MYSQL*)m_pkMysql), host, user, pass, dbname );
		Close();
		return false;
	}

	/*
	int result = mysql_set_character_set( (MYSQL*)m_pkMysql, "utf8" ); //"cp932"
	if( result != 0 ){
		_SetError( -1, "db set character set error" );
		Close();
		return false;
	}
	*/

	return true;
}

// DBコネクションを閉じます。
void CMySql::Close()
{
	_SetError(0);	// エラーをクリア //

	if( m_pkMysql ){
		mysql_close( (MYSQL*)m_pkMysql );
		m_pkMysql = NULL;
	}
}

void CMySql::Ping()
{
	if( m_pkMysql ){
		mysql_ping( (MYSQL*)m_pkMysql );
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// クエリ準備
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// mysql_real_escape_stringのラッパー //
unsigned long CMySql::EscapeString( char *to, const char *from, unsigned long length )
{
	if( !m_pkMysql )
		return 0;	// 接続していません。

	return mysql_real_escape_string( (MYSQL*)m_pkMysql, to, from, length );
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// クエリ
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// クエリ実行
int CMySql::RunQuery(
	const char*		_szQueryString,
	CMySqlValue		cValue0,
	CMySqlValue		cValue1,
	CMySqlValue		cValue2,
	CMySqlValue		cValue3,
	CMySqlValue		cValue4,
	CMySqlValue		cValue5,
	CMySqlValue		cValue6,
	CMySqlValue		cValue7
)
{
	CMySqlValue* aValues[] = {&cValue0, &cValue1, &cValue2, &cValue3, &cValue4, &cValue5, &cValue6, &cValue7};
	m_cRows.clear();
	_SetError(0);	// エラーをクリア //

	if( !m_pkMysql )
		return -1;	// 接続していません。

	// プレースホルダ展開 (自前) //
	const char* szQueryString = NULL;
	if(strchr(_szQueryString, '?')){
		// バッファ確保 //
		static std::vector<char> vBuf;
		int nSize = (int)strlen(_szQueryString);
		for(int i = 0; i < _countof(aValues); i++){
			nSize += (int)aValues[i]->GetEscaped(m_pkMysql).length();
		}
		vBuf.resize(nSize + 8 * 2 + 1); // 8*2: ﾀﾞﾌﾞﾙｸｫｰﾃｰｼｮﾝ  1: 終端ﾇﾙ //
		// 展開 //
		int iValueIndex = 0;
		char* p = &vBuf[0];
		const char* q = _szQueryString;
		while(*q){
			if(*q == '?'){
				q++;
				assert(iValueIndex < _countof(aValues));
				CMySqlValue* pcValue = aValues[iValueIndex];
				assert(pcValue->IsValid());
				if(pcValue->IsString())*p++ = '"';
				strcpy(p, pcValue->GetEscaped(m_pkMysql).c_str()); p += pcValue->GetEscaped(m_pkMysql).length();
				if(pcValue->IsString())*p++ = '"';
				iValueIndex++;
			}
			else{
				*p++ = *q++;
			}
		}
		*p++ = '\0';
		// 使用 //
		szQueryString = &vBuf[0];
	}
	else{
		szQueryString = _szQueryString;
	}
	
	// クエリの発行
	KLOG(_QUERY, "%s", szQueryString);
	int result = mysql_real_query( (MYSQL*)m_pkMysql, szQueryString, (unsigned int)strlen(szQueryString) );
	if( result != 0 ){
		_SetError( -1, "mysql_query() failed (%s)!!,sql(%s)", mysql_error((MYSQL*)m_pkMysql), szQueryString );
		KLOG(_ERROR, "mysql_query() failed (%s)!!,sql(%s)", mysql_error((MYSQL*)m_pkMysql), szQueryString );
		return -1;
	}

	// 直前のクエリの処理件数を返す //
	if(strnicmp(_szQueryString, "select", strlen("select")) == 0){
//		MYSQL_RES* pkRes = mysql_use_result((MYSQL*)m_pkMysql);	// レコードセット //
//		int iRows = (int)mysql_num_rows(pkRes);
//		mysql_free_result(pkRes);	// メモリ解放
		_GetRows(&m_cRows);
		return (int)m_cRows.size();
	}
	else{
		return (int)mysql_affected_rows( (MYSQL*)m_pkMysql );
	}
}

// SELECT 実行
bool CMySql::_GetRows(
	std::vector< std::vector<std::wstring> >* pvRecords
)
{
	if(!m_pkMysql)return false;

	// データの取得開始（順次）
	//MYSQL_ROW		record;
	//MYSQL_FIELD*	fields;
	//MYSQL_DATA		data;
	MYSQL_RES*	pkRes = mysql_use_result((MYSQL*)m_pkMysql);	// レコードセット //
	if( !pkRes ){
		// SQL失敗？
		_SetError( -1, "mysql_use_result() failed (%s)!!", mysql_error((MYSQL*)m_pkMysql) );
		return false;
	}

	if( pkRes ){
		int iFieldCount = mysql_num_fields(pkRes);

		MYSQL_ROW		record;
		while( record = mysql_fetch_row(pkRes) ){
			// レコードを vector に変換 //
			std::vector<std::wstring> vRecord;
			for(int i = 0; i < iFieldCount; i++){
				vRecord.push_back(cpp_mbstowcs(record[i]?record[i]:"NULL", CP_UTF8));
			}
			// レコードリストに追加 //
			pvRecords->push_back(vRecord);
		}
	}

	if( pkRes ){
		mysql_free_result(pkRes);	// メモリ解放
		pkRes = NULL;
	}

	return true;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// クエリ後
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 直前のINSERT文のオートインクリメント型idを取得 //
int CMySql::GetRecentInsertId()
{
	if( !m_pkMysql )
		return 0;	// 接続していません。

	return (int)mysql_insert_id((MYSQL*)m_pkMysql);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// エラー情報
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// エラーセット //
void CMySql::_SetError(int err_no, const char* msg, ...)
{
	m_err_no = err_no;
	va_list v;
	va_start(v, msg);
	vsprintf(m_err_msg, msg, v);
	va_end(v);
}
