#pragma once

#include <time.h>

class Time{
public:
	Time();
	virtual ~Time()
	{
	}
public:
	int getYear()		{ return t.tm_year+1900;	}
	int getMonth()		{ return t.tm_mon+1;		}
	int getDay()		{ return t.tm_mday;			}
	int getHours()		{ return t.tm_hour;			}
	int getMinutes()	{ return t.tm_min;			}
	int getSeconds()	{ return t.tm_sec;			}
	int getWeek()		{ return t.tm_wday;			}
	int getDaysInYear()	{ return t.tm_yday+1;		}
private:
	tm t;
};

