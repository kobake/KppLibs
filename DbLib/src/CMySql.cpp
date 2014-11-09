#include "CMySql.h"
//// my_global �����stl ���C���N���[�h���Ȃ��ƃ����N�G���[�ɂȂ�H
#include <my_global.h>	// bool �g���Ȃ��Ȃ�܂��B
#include <mysql.h>
#ifdef bool
	#undef bool
#endif
#include "util/mywstring.h"
#include "util/CLog.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ���C�u����
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
// �l
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
// �������ƏI��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CMySql::CMySql()
{
	MysqlInit();
	m_pkMysql = 0;
	m_err_no = 0;			// �G���[�ԍ��A0�Ȃ�G���[�Ȃ�//
	strcpy( m_err_msg, "" );
}

CMySql::~CMySql()
{
	Close();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �ڑ�
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// DB�R�l�N�V�������J���܂��B //
bool CMySql::Open( const char* host, const char* user, const char* pass, const char* dbname, int port )
{
	// port�w�肪������΃f�t�H���g���g�� //
	if(port <= 0){
		port = 3306;
	}

	_SetError(0);	// �G���[���N���A //
	Close();	// �O�̂��ߑO�̐ڑ�����܂��B

	m_pkMysql = mysql_init( NULL );
	if( !m_pkMysql ){
		_SetError( -1, "mysql_init() returned null" );
		Close();
		return false;
	}

	// MySQL�T�[�o�֐ڑ�
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

// DB�R�l�N�V��������܂��B
void CMySql::Close()
{
	_SetError(0);	// �G���[���N���A //

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
// �N�G������
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// mysql_real_escape_string�̃��b�p�[ //
unsigned long CMySql::EscapeString( char *to, const char *from, unsigned long length )
{
	if( !m_pkMysql )
		return 0;	// �ڑ����Ă��܂���B

	return mysql_real_escape_string( (MYSQL*)m_pkMysql, to, from, length );
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �N�G��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �N�G�����s
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
	_SetError(0);	// �G���[���N���A //

	if( !m_pkMysql )
		return -1;	// �ڑ����Ă��܂���B

	// �v���[�X�z���_�W�J (���O) //
	const char* szQueryString = NULL;
	if(strchr(_szQueryString, '?')){
		// �o�b�t�@�m�� //
		static std::vector<char> vBuf;
		int nSize = (int)strlen(_szQueryString);
		for(int i = 0; i < _countof(aValues); i++){
			nSize += (int)aValues[i]->GetEscaped(m_pkMysql).length();
		}
		vBuf.resize(nSize + 8 * 2 + 1); // 8*2: ����ٸ��ð���  1: �I�[�� //
		// �W�J //
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
		// �g�p //
		szQueryString = &vBuf[0];
	}
	else{
		szQueryString = _szQueryString;
	}
	
	// �N�G���̔��s
	KLOG(_QUERY, "%s", szQueryString);
	int result = mysql_real_query( (MYSQL*)m_pkMysql, szQueryString, (unsigned int)strlen(szQueryString) );
	if( result != 0 ){
		_SetError( -1, "mysql_query() failed (%s)!!,sql(%s)", mysql_error((MYSQL*)m_pkMysql), szQueryString );
		KLOG(_ERROR, "mysql_query() failed (%s)!!,sql(%s)", mysql_error((MYSQL*)m_pkMysql), szQueryString );
		return -1;
	}

	// ���O�̃N�G���̏���������Ԃ� //
	if(strnicmp(_szQueryString, "select", strlen("select")) == 0){
//		MYSQL_RES* pkRes = mysql_use_result((MYSQL*)m_pkMysql);	// ���R�[�h�Z�b�g //
//		int iRows = (int)mysql_num_rows(pkRes);
//		mysql_free_result(pkRes);	// ���������
		_GetRows(&m_cRows);
		return (int)m_cRows.size();
	}
	else{
		return (int)mysql_affected_rows( (MYSQL*)m_pkMysql );
	}
}

// SELECT ���s
bool CMySql::_GetRows(
	std::vector< std::vector<std::wstring> >* pvRecords
)
{
	if(!m_pkMysql)return false;

	// �f�[�^�̎擾�J�n�i�����j
	//MYSQL_ROW		record;
	//MYSQL_FIELD*	fields;
	//MYSQL_DATA		data;
	MYSQL_RES*	pkRes = mysql_use_result((MYSQL*)m_pkMysql);	// ���R�[�h�Z�b�g //
	if( !pkRes ){
		// SQL���s�H
		_SetError( -1, "mysql_use_result() failed (%s)!!", mysql_error((MYSQL*)m_pkMysql) );
		return false;
	}

	if( pkRes ){
		int iFieldCount = mysql_num_fields(pkRes);

		MYSQL_ROW		record;
		while( record = mysql_fetch_row(pkRes) ){
			// ���R�[�h�� vector �ɕϊ� //
			std::vector<std::wstring> vRecord;
			for(int i = 0; i < iFieldCount; i++){
				vRecord.push_back(cpp_mbstowcs(record[i]?record[i]:"NULL", CP_UTF8));
			}
			// ���R�[�h���X�g�ɒǉ� //
			pvRecords->push_back(vRecord);
		}
	}

	if( pkRes ){
		mysql_free_result(pkRes);	// ���������
		pkRes = NULL;
	}

	return true;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �N�G����
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ���O��INSERT���̃I�[�g�C���N�������g�^id���擾 //
int CMySql::GetRecentInsertId()
{
	if( !m_pkMysql )
		return 0;	// �ڑ����Ă��܂���B

	return (int)mysql_insert_id((MYSQL*)m_pkMysql);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �G���[���
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �G���[�Z�b�g //
void CMySql::_SetError(int err_no, const char* msg, ...)
{
	m_err_no = err_no;
	va_list v;
	va_start(v, msg);
	vsprintf(m_err_msg, msg, v);
	va_end(v);
}
