#include "_required.h"
#include "CRegValue.h"
#include "CRegException.h"
#include "CRegType.h"
#include <StringLib.h>

static const wchar_t* skip_split(const wchar_t* p)
{
	while(*p==L' ')p++;
	return p;
}
static const wchar_t* skip_token(const wchar_t* p)
{
	while(*p!=L' ' && *p!=L'\0')p++;
	return p;
}


unsigned int atoi16(const wchar_t *str,int keta=8)
{
	const wchar_t *p=str;
	int ret=0;
	int c,n;
	const int bit_max=32;
	int bit=0;
	while(*p!=L'\0'){
		if(bit>=bit_max)goto err; //桁数オーバー
		c=*p;
		if(c>=L'0' && c<=L'9')n=c-L'0';
		else if(c>=L'a' && c<=L'f')n=10+c-L'a';
		else if(c>=L'A' && c<=L'F')n=10+c-L'A';
		else goto err; //無効な文字
		ret<<=4;
		ret|=n;
		p++;
		bit+=4;
		if(--keta<=0)break;
	}
	return ret;
err:
	return 0;
}

unsigned int atoi_ex(const wchar_t *str)
{
	if(str[0]==L'0' && (str[1]==L'x' || str[1]==L'X')){
		return atoi16(&str[2]);
	}else{
		return (unsigned int)_wtoi(str);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CRegValue::CRegValue()
: m_dwType(REG_NONE)
{
}

CRegValue::CRegValue(const wchar_t* source,const CRegType& _type)
: m_dwType(_type.getDword())
{
	//文字列
	if(m_dwType==REG_SZ || m_dwType==REG_EXPAND_SZ){
		int n=(int)wcslen(source)+1;
		m_vData.resize(n*sizeof(wchar_t),0);
		wcscpy((wchar_t*)&m_vData[0],source);
	}
	//バイナリ
	//空白区切りの2桁16進
	//ex: 00 0f 1a 72
	else if(m_dwType==REG_BINARY){
		const wchar_t* p=source;
		const wchar_t* q;
		while(1){
			p=skip_split(p);
			q=skip_token(p);
			if(q-p>2)p=q-2; //2桁以上ある場合は、下2桁のみを用いる
			if(q-p<=0)break;
			m_vData.push_back((byte)atoi16(p,(int)(q-p)));
		}
	}
	//DWORD
	//数値
	//ex: 341
	//ex: 0x1b9
	else if(m_dwType==REG_DWORD){
		m_vData.resize(sizeof(DWORD),0);
		DWORD d=atoi_ex(source);
		memcpy(&m_vData[0],&d,sizeof(DWORD));
	}
	//例外
	else{
		throw CRegException(L"CRegValue: unsupported reg m_dwType");
	}
}

CRegValue::CRegValue(const void* _data,int _ndata,const CRegType& _type)
: m_vData((const byte*)_data,(const byte*)_data+_ndata)
, m_dwType(_type.getDword())
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 情報
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
std::wstring CRegValue::ToString() const
{
	if(m_dwType==REG_NONE){
		return L"";
	}
	else if(m_dwType==REG_SZ){
		return m_vData.size()?(const wchar_t*)&m_vData[0]:L"";
	}
	else if(m_dwType==REG_EXPAND_SZ){
		//環境変数の展開は省略
		return m_vData.size()?(const wchar_t*)&m_vData[0]:L"";
	}
	else if(m_dwType==REG_BINARY){
		std::wstring ret;
		for(int i=0;i<(int)m_vData.size();i++){
			wchar_t buf[32];
			swprintf(buf, _countof(buf), L"%02X", m_vData[i]);
			ret += buf;
		}
		return ret;
	}
	else if(m_dwType==REG_DWORD){
		return tmp_itow(m_dwType);
	}
	else{
		return L"Unknown type data";
	}
}
