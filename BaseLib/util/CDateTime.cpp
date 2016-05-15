#include "_required.h"
#include <cstdio>
#include "CDateTime.h"
#include <StringLib.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �j��                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
const wchar_t* CWeek::GetName() const
{
	const wchar_t* table[] = {
		L"Sun",
		L"Mon",
		L"Tue",
		L"Wed",
		L"Thu",
		L"Fri",
		L"Sat",
	};
	return table[m_nWeek];
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CDateTime::CDateTime()
{
	SetCurrent();
}

CDateTime::CDateTime(time_t t)
{
	m_long = t;
	m_struct = *localtime(&m_long);
}

CDateTime::CDateTime(int Y,int M,int D)
{
	Set(Y, M, D, 0, 0, 0);
}

CDateTime::CDateTime(int Y,int M,int D,int h,int m,int s)
{
	Set(Y, M, D, h, m, s);
}

//�����Z
void CDateTime::OffsetDay(int nOffsetDay)
{
	m_long += nOffsetDay * 24 * 60 * 60;
	m_struct = *localtime(&m_long);
}

//WIN32
void CDateTime::Set(const FILETIME& ft)
{
	//���Q�l�Fhttp://support.microsoft.com/kb/167296/ja
	unsigned __int64 tmp = ft.dwLowDateTime | (((unsigned __int64)ft.dwHighDateTime)<<32);
	if(tmp>=116444736000000000)tmp-=116444736000000000;
	else tmp = 0;
	tmp /= 10000000;
	m_long = (time_t)tmp;
	m_struct = *localtime(&m_long);
}

void CDateTime::SetZero()
{
	m_long = 0;
	memset(&m_struct, 0, sizeof(m_struct));
}

void CDateTime::SetCurrent()
{
	time(&m_long);
	m_struct = *localtime(&m_long);
}

void CDateTime::Set(int Y, int M, int D, int h, int m, int s)
{
	m_struct.tm_sec=	s;
	m_struct.tm_min=	m;
	m_struct.tm_hour=	h;
	m_struct.tm_mday=	D;				// day of the month - [1,31]
	m_struct.tm_mon=	M-1;			// months since January - [0,11]
	m_struct.tm_year=	Y-1900;			// years since 1900
	m_struct.tm_wday=	0;		//����
	m_struct.tm_yday=	0;		//����
	m_struct.tm_isdst=	0;		//�W�����ԁi�Ď��Ԃł͂Ȃ��j

	m_long = mktime(&m_struct);	//1970/01/01 00:00:00(GMT) �ȍ~�̌o�ߕb��
	tm* p = localtime(&m_long);
	if(!p){
		throw std::exception("localtime failed.");
	}
	m_struct = *p;
}

void CDateTime::SetCompiledDate(const char* szCompiledDate, const char* szCompiledTime)
{
	SetZero();
	// -- -- �N���� -- -- //
	// �����e�[�u��
	static std::map<std::string, int> months;
	if(months.size() == 0){
		months["Jan"] = 1;
		months["Feb"] = 2;
		months["Mar"] = 3;
		months["Apr"] = 4;
		months["May"] = 5;
		months["Jun"] = 6;
		months["Jul"] = 7;
		months["Aug"] = 8;
		months["Sep"] = 9;
		months["Oct"] = 10;
		months["Nov"] = 11;
		months["Dec"] = 12;
	}
	// Nov 10 2012
	std::vector<std::string> v = strsplit(szCompiledDate, ' ');
	if(v.size() != 3)return;
	// ��
	std::map<std::string, int>::const_iterator itr = months.find(v[0]);
	if(itr == months.end())return;
	int month = itr->second;
	// ��
	int day = atoi(v[1].c_str());
	// �N
	int year = atoi(v[2].c_str());
	// -- -- �N���� -- -- //
	v = strsplit(szCompiledTime, ':');
	v.push_back("0");
	v.push_back("0");
	v.push_back("0");
	int hour = atoi(v[0].c_str());
	int min = atoi(v[1].c_str());
	int sec = atoi(v[2].c_str());
	// -- -- �ݒ� -- -- //
	Set(year, month, day, hour, min, sec);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �擾                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// 1970/01/01(GMT) �ȍ~�̌o�ߓ���  �����̎��_�ŁA
int CDateTime::GetDaysFromZero() const
{
	return (int)((m_long+9*60*60/*JST*/)/60/60/24);
}

// 1970/01/01(GMT) �ȍ~�̌o�ߓ�����n��
void CDateTime::SetDaysFromZero(int nDays)
{
	m_long = nDays * 24 * 60 * 60;
	m_long -= 9*60*60; //JST
	m_struct = *localtime(&m_long);
}


CDateTime::~CDateTime()
{
}

//�\������
/*
	YYYY
	MM
	DD
	hh
	mm
	ss
*/
std::wstring CDateTime::ToString(const wchar_t* szFormat) const
{
	wchar_t buf[256];
	wcscpy_s(buf,_countof(buf),szFormat);
	wcsreplace(buf,L"YYYY",itow_radix(GetYear()  ,10,4)); wcsreplace(buf,L"Y",itow_radix(GetYear()  ,10,1));
	wcsreplace(buf,L"MM"  ,itow_radix(GetMonth() ,10,2)); wcsreplace(buf,L"M",itow_radix(GetMonth() ,10,1));
	wcsreplace(buf,L"DD"  ,itow_radix(GetDay()   ,10,2)); wcsreplace(buf,L"D",itow_radix(GetDay()   ,10,1));
	wcsreplace(buf,L"hh"  ,itow_radix(GetHour()  ,10,2)); wcsreplace(buf,L"h",itow_radix(GetHour()  ,10,1));
	wcsreplace(buf,L"mm"  ,itow_radix(GetMinute(),10,2)); wcsreplace(buf,L"m",itow_radix(GetMinute(),10,1));
	wcsreplace(buf,L"ss"  ,itow_radix(GetSecond(),10,2)); wcsreplace(buf,L"s",itow_radix(GetSecond(),10,1));
	wcsreplace(buf,L"W"   ,GetWeek().GetName());
	return buf;
}
