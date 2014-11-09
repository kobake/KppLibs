#include "common/_required.h"
#include "cpp_swprintf.h"
#include <string>
using namespace std;


class CField{
public:
	//書式指定: flag
	inline const wchar_t* _ParseFieldFlag(const wchar_t* p)
	{
		while(*p){
			wchar_t c=*p;
			if(c==L'-' || c==L'+' || c==L'0' || c==L' ' || c==L'#'){ p++; continue; }
			break;
		}
		return p;
	}
	//書式指定: width
	inline const wchar_t* _ParseFieldWidth(const wchar_t* p)
	{
		if(*p>=L'1' && *p<=L'9')p++; else return p; //一桁目は0を受け付けない
		while(*p>=L'0' && *p<=L'9')p++;
		return p;
	}
	//書式指定: precision
	inline const wchar_t* _ParseFieldPrecision(const wchar_t* p)
	{
		if(*p==L'.')p++; else return p; //ドットで始まる文字列のみ受け付ける
		while(*p>=L'0' && *p<=L'9')p++; //よくわからんのでとりあえず全数字を受け付ける
		return p;
	}
	//書式指定: prefix
	inline const wchar_t* _ParseFieldPrefix(const wchar_t* p)
	{
		if(*p==L't')return p+1; //独自拡張
		if(*p==L'h')return p+1;
		if(p[0]==L'l' && p[1]==L'l')return p+2;
		if(*p==L'l')return p+1;
		if(p[0]==L'I' && p[1]==L'3' && p[2]==L'2')return p+3;
		if(p[0]==L'I' && p[1]==L'6' && p[2]==L'4')return p+3;
		if(*p==L'I')return p+1;
		return p;
	}
	//書式指定: type
	inline const wchar_t* _ParseFieldType(const wchar_t* p)
	{
		m_nType = *p++;
		return p;
	}
	//使用
	inline bool IsValidField() const
	{
		return m_nType!=0 && wcschr(L"cCdiouxXeEfgGaAnpsS",m_nType)!=NULL;
	}
	inline bool IsStringField() const
	{
		return wcschr(L"sS",m_nType)!=NULL;
	}
	inline int GetLength() const
	{
		return (int)wcslen(m_szField);
	}
	inline wchar_t GetType() const
	{
		return m_nType;
	}
	inline const wchar_t* GetField() const
	{
		return m_szField;
	}
	//初期化
	inline const wchar_t* Parse(const wchar_t* _p)
	{
		const wchar_t* p = _p;
		if(*p==L'%'){
			const wchar_t* pBegin = p; //'%'から始まる
			p++;
			p=this->_ParseFieldFlag(p);
			p=this->_ParseFieldWidth(p);
			p=this->_ParseFieldPrecision(p);
			p=this->_ParseFieldPrefix(p);
			p=this->_ParseFieldType(p);
			if(this->IsValidField()){
				const wchar_t* pEnd = p;

				//フィールドをメンバ変数にコピー -> m_szField
				if(pEnd-pBegin>=_countof(m_szField))goto err; //長すぎるフィールドは無効とする
				wcsncpy(m_szField,pBegin,pEnd-pBegin);
				m_szField[pEnd-pBegin] = 0;

				return p;
			}
		}
err:
		//有効なフィールドを取得できなかった場合は、そのまま返す
		m_szField[0] = 0;
		m_nType = 0;
		return _p;
	}
	//マクロ
	inline static bool IS_FIELD_BEGIN(wchar_t c)
	{
		return c==L'%';
	}
private:
	wchar_t			m_nType;
	wchar_t			m_szField[64];
};





static void my_va_forward(va_list& v, const wchar_t* field)
{
	if(*field==0)return;
	const wchar_t* field_end=wcschr(field,0)-1;
	switch(*field_end){
	case L's':
	case L'S':
	case L'p':
		va_arg(v,void*);
		break;
	case L'd':
	case L'i':
	case L'o':
	case L'u':
	case L'x':
	case L'X':
		va_arg(v,int);
		break;
	case 'c':
		if(field_end-1>=field && *(field_end-1)==L'h')va_arg(v,int);
		else va_arg(v,int);
		break;
	case 'C':
		if(field_end-1>=field && *(field_end-1)==L'l')va_arg(v,wchar_t);
		else if(field_end-1>=field && *(field_end-1)==L'w')va_arg(v,wchar_t);
		else va_arg(v,int);
		break;
	case L'e':
	case L'E':
	case L'f':
	case L'g':
	case L'G':
	case L'a':
	case L'A':
		va_arg(v,double);
		break;
	}
}


//! wstringで結果を返す swprintf
wstring cpp_vswprintf(const wchar_t* format, va_list& v)
{
	wstring strRet;
	const wchar_t* src=format; //変換元ワーク変数
	while(*src){
		//書式指定フィールドを取得
		if(CField::IS_FIELD_BEGIN(*src)){
			CField cField;
			src = cField.Parse(src);

			if(cField.IsValidField()){
				//変換処理
				va_list tmp_v=v; //※vをコピーして用いる
				if(cField.IsStringField()){
					//文字列は自前で結合
					const wchar_t* p = va_arg(tmp_v,const wchar_t*);
					strRet+=p;
				}
				else{
					//数値は標準ライブラリに委譲
					wchar_t buf[64];
					int ret=vswprintf_s(buf,_countof(buf),cField.GetField(),tmp_v);
					if(ret!=-1){
						buf[ret] = 0;
					}
					strRet+=buf;
				}

				//vを進める。自信なっしんぐ
				my_va_forward(v,cField.GetField());
			}
			else{
				//有効な型フィールドではなかったので、そのまんま出力しちゃう
				strRet += *src++;
			}
		}
		else{
			//無変換
			strRet += *src++;
		}
	}
	//結果
	return strRet;
}

wstring cpp_swprintf(const wchar_t* format, ...)
{
	va_list v;
	va_start(v,format);
	wstring ret = cpp_vswprintf(format,v);
	va_end(v);
	return ret;
}



