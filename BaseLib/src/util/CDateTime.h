#pragma once

#include <ctime>
#include <string>
#include <windows.h> //FILETIME

class CWeek{
public:
	CWeek(int nWeek) : m_nWeek(nWeek) { }
	const int GetIndex() const{ return m_nWeek; }
	const wchar_t* GetName() const;
private:
	int m_nWeek;
};

class CDateTime{
public:
	CDateTime();
	CDateTime(time_t t);
	CDateTime(int y,int m,int d);
	CDateTime(int Y,int M,int D,int h,int m,int s);
	CDateTime(const FILETIME& ft){ Set(ft); }
	virtual ~CDateTime();
public:
	// -- -- 設定 -- -- //
	void SetDaysFromZero(int nDays);
	//加減算
	void OffsetDay(int nOffsetDay);
	//WIN32
	void Set(const FILETIME& ft);
	void SetZero();
	void SetCurrent();
	void Set(int Y, int M, int D, int h, int m, int s);
	void SetCompiledDate(const char* szCompiledDate, const char* szCompiledTime = "00:00:00");

	// -- -- 取得 -- -- //
	//日付
	int GetYear() const			{ return m_struct.tm_year+1900;	}
	int GetMonth() const		{ return m_struct.tm_mon+1;		}
	int GetDay() const			{ return m_struct.tm_mday;		}
	//時刻
	int GetHour() const			{ return m_struct.tm_hour;		}
	int GetMinute() const		{ return m_struct.tm_min;		}
	int GetSecond() const		{ return m_struct.tm_sec;		}
	//その他
	CWeek GetWeek() const		{ return m_struct.tm_wday;		}	//0:日〜6:土
	int GetDaysInYear() const	{ return m_struct.tm_yday+1;	}
	//
	int GetDaysFromZero() const;									//GMT 1970/01/01以降の経過日数

	// -- -- その他 -- -- //
	//演算子
	bool operator < (const CDateTime& rhs) const{ return this->m_long < rhs.m_long; }
	int Compare(const CDateTime& rhs) const
	{
		__int64 tmp = (__int64)this->m_long - (__int64)rhs.m_long;
		if(tmp>0)return 1;
		else if(tmp<0)return -1;
		else return 0;
	}
		
	//表示書式
	std::wstring ToString(
		const wchar_t* szFormat = L"YYYY/MM/DD (W) hh:mm:ss"
	) const;

	// 生 //
	time_t	GetUnixTime() const	{ return m_long; }
private:
	time_t	m_long;		//1970/01/01 00:00:00以降の経過秒数
	tm		m_struct;
};

