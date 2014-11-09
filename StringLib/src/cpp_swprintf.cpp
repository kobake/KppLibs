#include "common/_required.h"
#include "cpp_swprintf.h"
#include <string>
using namespace std;


class CField{
public:
	//�����w��: flag
	inline const wchar_t* _ParseFieldFlag(const wchar_t* p)
	{
		while(*p){
			wchar_t c=*p;
			if(c==L'-' || c==L'+' || c==L'0' || c==L' ' || c==L'#'){ p++; continue; }
			break;
		}
		return p;
	}
	//�����w��: width
	inline const wchar_t* _ParseFieldWidth(const wchar_t* p)
	{
		if(*p>=L'1' && *p<=L'9')p++; else return p; //�ꌅ�ڂ�0���󂯕t���Ȃ�
		while(*p>=L'0' && *p<=L'9')p++;
		return p;
	}
	//�����w��: precision
	inline const wchar_t* _ParseFieldPrecision(const wchar_t* p)
	{
		if(*p==L'.')p++; else return p; //�h�b�g�Ŏn�܂镶����̂ݎ󂯕t����
		while(*p>=L'0' && *p<=L'9')p++; //�悭�킩���̂łƂ肠�����S�������󂯕t����
		return p;
	}
	//�����w��: prefix
	inline const wchar_t* _ParseFieldPrefix(const wchar_t* p)
	{
		if(*p==L't')return p+1; //�Ǝ��g��
		if(*p==L'h')return p+1;
		if(p[0]==L'l' && p[1]==L'l')return p+2;
		if(*p==L'l')return p+1;
		if(p[0]==L'I' && p[1]==L'3' && p[2]==L'2')return p+3;
		if(p[0]==L'I' && p[1]==L'6' && p[2]==L'4')return p+3;
		if(*p==L'I')return p+1;
		return p;
	}
	//�����w��: type
	inline const wchar_t* _ParseFieldType(const wchar_t* p)
	{
		m_nType = *p++;
		return p;
	}
	//�g�p
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
	//������
	inline const wchar_t* Parse(const wchar_t* _p)
	{
		const wchar_t* p = _p;
		if(*p==L'%'){
			const wchar_t* pBegin = p; //'%'����n�܂�
			p++;
			p=this->_ParseFieldFlag(p);
			p=this->_ParseFieldWidth(p);
			p=this->_ParseFieldPrecision(p);
			p=this->_ParseFieldPrefix(p);
			p=this->_ParseFieldType(p);
			if(this->IsValidField()){
				const wchar_t* pEnd = p;

				//�t�B�[���h�������o�ϐ��ɃR�s�[ -> m_szField
				if(pEnd-pBegin>=_countof(m_szField))goto err; //��������t�B�[���h�͖����Ƃ���
				wcsncpy(m_szField,pBegin,pEnd-pBegin);
				m_szField[pEnd-pBegin] = 0;

				return p;
			}
		}
err:
		//�L���ȃt�B�[���h���擾�ł��Ȃ������ꍇ�́A���̂܂ܕԂ�
		m_szField[0] = 0;
		m_nType = 0;
		return _p;
	}
	//�}�N��
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


//! wstring�Ō��ʂ�Ԃ� swprintf
wstring cpp_vswprintf(const wchar_t* format, va_list& v)
{
	wstring strRet;
	const wchar_t* src=format; //�ϊ������[�N�ϐ�
	while(*src){
		//�����w��t�B�[���h���擾
		if(CField::IS_FIELD_BEGIN(*src)){
			CField cField;
			src = cField.Parse(src);

			if(cField.IsValidField()){
				//�ϊ�����
				va_list tmp_v=v; //��v���R�s�[���ėp����
				if(cField.IsStringField()){
					//������͎��O�Ō���
					const wchar_t* p = va_arg(tmp_v,const wchar_t*);
					strRet+=p;
				}
				else{
					//���l�͕W�����C�u�����ɈϏ�
					wchar_t buf[64];
					int ret=vswprintf_s(buf,_countof(buf),cField.GetField(),tmp_v);
					if(ret!=-1){
						buf[ret] = 0;
					}
					strRet+=buf;
				}

				//v��i�߂�B���M�Ȃ�����
				my_va_forward(v,cField.GetField());
			}
			else{
				//�L���Ȍ^�t�B�[���h�ł͂Ȃ������̂ŁA���̂܂�܏o�͂����Ⴄ
				strRet += *src++;
			}
		}
		else{
			//���ϊ�
			strRet += *src++;
		}
	}
	//����
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



