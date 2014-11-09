#include "common/_required.h"
#include "CLog.h"
#include <stdio.h>
#include <map>
#include "common/CAppInfo.h"
#include <StringLib.h>
#include "util/file.h" // feixst

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// CLogStream
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CLogStream::CLogStream(const wchar_t* szFileNameTemplate, int nRotationSize, bool bLogHold)
: m_nRef(0)
, m_strFileNameTemplate(szFileNameTemplate)
, m_fp(NULL)
, m_nRotationSize(nRotationSize)
, m_bLogHold(bLogHold)
{
	m_strFileNameTemplate = cpp_wcsreplace(m_strFileNameTemplate, L"\\", L"/");
	ReOpen();
}

CLogStream::~CLogStream()
{
	if(m_fp){
		::fclose(m_fp);
	}
}

void CLogStream::ReOpen()
{
	if(m_fp){
		::fclose(m_fp);
	}

	// 現在日時
	SYSTEMTIME st;
	GetLocalTime(&st);

	//実際のファイル名生成
	wchar_t szFile[1024];
	wchar_t* p = szFile;
	wchar_t* end = p + _countof(szFile) - 1;
	const wchar_t* q = m_strFileNameTemplate.c_str();
	while(*q && p < end-4){
		if(0){}
		else if(wcsncmp(q, L"$yyyy", 5)==0){ p += swprintf(p, L"%04d", st.wYear);   q += 5; }
		else if(wcsncmp(q, L"$mm",   3)==0){ p += swprintf(p, L"%02d", st.wMonth);  q += 3; }
		else if(wcsncmp(q, L"$dd",   3)==0){ p += swprintf(p, L"%02d", st.wDay);    q += 3; }
		else if(wcsncmp(q, L"$HH",   3)==0){ p += swprintf(p, L"%02d", st.wHour);   q += 3; }
		else if(wcsncmp(q, L"$MM",   3)==0){ p += swprintf(p, L"%02d", st.wMinute); q += 3; }
		else if(wcsncmp(q, L"$SS",   3)==0){ p += swprintf(p, L"%02d", st.wSecond); q += 3; }
		else{
			*p++ = *q++;
		}
	}
	*p = 0;

	// 被らない $N を探す //
	if(wcsstr(szFile, L"$N")){
		wchar_t szN[32] = L"";
		for(int i = 0; i <= 9; i++){
			_itow(i, szN, 10);
			std::wstring strNewFile = cpp_wcsreplace(szFile, L"$N", szN);
			if(!fexist(strNewFile.c_str()) || i == 9){
				wcscpy_s(szFile, _countof(szFile), strNewFile.c_str());
				break;
			}
		}
	}

	//開く
	m_strCurrentFileName = szFile;
	if(m_bLogHold){
		m_fp = ::_wfopen(m_strCurrentFileName.c_str(), L"at");
	}
}

void CLogStream::PrintLn(const char* str)
{
	FILE* fp = m_fp;
	if(m_bLogHold){
		fp = m_fp;
	}
	else{
		fp = ::_wfopen(m_strCurrentFileName.c_str(), L"at");
	}

	// ファイルオープンに失敗してる場合でもエラー扱いとはしない。
	if(!fp)return;

	// カキコ //
	fprintf(fp, "%hs\n", str);
	fflush(fp);

	if(m_bLogHold){
		// 512KBを越えたらローテート。
		long n = ::ftell(m_fp);
		if(n >= m_nRotationSize){
			this->ReOpen();
		}
	}
	else{
		fclose(fp);
	}
}
