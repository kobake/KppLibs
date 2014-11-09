#include "_required.h"
#include "CRegKey.h"
#include "CRegException.h"
#include "CRegHive.h"
#include "CRegValue.h"
#include "CRegType.h"
using namespace std;

static void _OnError(int nResult, const wchar_t* szFunction, const wchar_t* szRegKey)
{
	switch(nResult){
	case ERROR_FILE_NOT_FOUND:
		throw CRegException_KeyNotFound(L"%ls - Key not found. [%ls]", szFunction, szRegKey);
		break;
	case ERROR_BAD_PATHNAME:
		throw CRegException_BadPath(L"%ls - Bad path. [%ls]", szFunction, szRegKey);
		break;
	default:
		throw CRegException(L"%ls - failed. [%ls]", szFunction, szRegKey);
		break;
	}
}		

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CRegKey::CRegKey(const CRegHive& hive,const wchar_t* key_name,bool to_create)
: m_hKey(NULL)
{
	Open(hive,key_name,to_create);
}

CRegKey::CRegKey(const wchar_t* szKeyName)
: m_hKey(NULL)
{
	Open(szKeyName);
}

CRegKey::CRegKey(const CRegKey& cParentKey, const wchar_t* szChildKey)
: m_hKey(NULL)
{
	Open(cParentKey, szChildKey);
}

CRegKey::CRegKey()
{
	m_hKey=NULL;
}

CRegKey::~CRegKey()
{
	try{
		Close();
	}catch(const CRegException&){
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// オープン・クローズ
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CRegKey::Open(const CRegHive& hive,const wchar_t* key_name,bool to_create)
{
	Close();

	m_strSummary = (wstring)hive.ToString() + L"\\" + key_name;

	LONG ret;
	if(!to_create){
		//既存のキーを開く
		ret=RegOpenKeyEx(hive.getHandle(),key_name,0,KEY_ALL_ACCESS,&m_hKey);
	}else{
		//既存のキーを開く。存在しなければ作成する
		DWORD d;
		ret=RegCreateKeyEx(hive.getHandle(),key_name,0,L"",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&m_hKey,&d);
	}

	//エラー処理
	if(ret!=ERROR_SUCCESS){
		_OnError(ret, L"CRegKey::Open", ToString());
	}
}

void CRegKey::Open(const wchar_t* _szKeyName)
{
	Close();

	m_strSummary = _szKeyName;

	//解釈 -> hHive, szKeyName
	HKEY hHive = NULL;
	const wchar_t* szKeyName = NULL;
	{
		CRegHive cHive(_szKeyName);
		hHive = cHive.getHandle();
		szKeyName = wcschr(_szKeyName, L'\\');
		if(szKeyName)szKeyName++;
	}

	//既存のキーを開く
	if(szKeyName){
		LONG ret = ::RegOpenKeyEx(hHive, szKeyName, 0, KEY_ALL_ACCESS/*KEY_READ*/, &m_hKey);
		if(ret != ERROR_SUCCESS){
			m_hKey = NULL;
			_OnError(ret, L"CRegKey::Open", ToString());
		}
	}
	//もしくはハイブをそのまま格納
	else{
		m_hKey = hHive;
	}
}

void CRegKey::Open(const CRegKey& cParentKey, const wchar_t* szChildKey)
{
	m_strSummary = (wstring)cParentKey.ToString() + L"\\" + szChildKey;

	//既存のキーを開く
	LONG ret = ::RegOpenKeyEx(cParentKey._GetHKEY(), szChildKey, 0, KEY_READ, &m_hKey);

	//エラー処理
	if(ret != ERROR_SUCCESS){
		m_hKey = NULL;
		_OnError(ret, L"CRegKey::Open", ToString());
	}
}

void CRegKey::Close()
{
	if(m_hKey){
		LONG ret=RegCloseKey(m_hKey);
		if(ret==ERROR_SUCCESS){
			m_hKey=NULL;
		}else{
			_OnError(ret, L"CRegKey::Close", ToString());
		}
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 値操作
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CRegKey::SetValue(const wchar_t* name,const CRegValue& value)
{
	LONG ret=RegSetValueEx(
		m_hKey,
		name,
		0,
		value.getType(),
		(const BYTE*)value.getData(),
		(DWORD)value.GetSize()
	);
	if(ret!=ERROR_SUCCESS){
		_OnError(ret, L"CRegKey::SetValue", ToString());
	}
}

CRegValue CRegKey::GetValue(const wchar_t* name)
{
	DWORD type;
	vector<BYTE> buf;
	DWORD nbuf;
	//バッファサイズ取得
	LONG ret = RegQueryValueEx(m_hKey,name,NULL,&type,NULL,&nbuf);
	if(ret != ERROR_SUCCESS){
		_OnError(ret, L"CRegKey::GetValue", ToString());
	}

	//読込
	if(nbuf>0){
		buf.resize(nbuf,0);
		RegQueryValueEx(m_hKey,name,NULL,&type,&buf[0],&nbuf);
	}
	//
	return CRegValue(buf.size()?&buf[0]:NULL,nbuf,type);
}

CRegValue CRegKey::TryGetValue(const wchar_t* name)
{
	try{
		return GetValue(name);
	}
	catch(CRegException){
		return CRegValue();
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 列挙
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
std::wstring CRegKey::EnumKey(int nIndex) const
{
	wchar_t buf[256];
	DWORD nbuf = _countof(buf);
	FILETIME t;
	LONG ret = ::RegEnumKeyEx(
		m_hKey,				//元のキー
		nIndex,				//サブキーのインデックス番号
		buf,				//サブキー名バッファ
		&nbuf,				//サブキー名バッファ長
		NULL,				//予約済み
		NULL,				//クラス名バッファ
		NULL,				//クラス名バッファ長
		&t					//最終書込時刻
	);
	if(ret != ERROR_SUCCESS){
		return L"";
	}
	return buf;
}

bool CRegKey::EnumValue(int nIndex, std::wstring* pstrNameBuf) const
{
	wchar_t buf[256];
	DWORD nbuf = _countof(buf);
	DWORD type = 0;
	LONG ret = ::RegEnumValue(
		m_hKey,			//元のキー
		nIndex,			//エントリのインデックス番号
		buf,			//エントリ名バッファ
		&nbuf,			//エントリ名バッファ長
		NULL,			//予約済み
		&type,			//データタイプ	
		NULL,			//データバッファ
		NULL			//データバッファ長
	);
	if(ret != ERROR_SUCCESS){
		*pstrNameBuf = L"";
		return false;
	}
	*pstrNameBuf = buf;
	return true;
}

