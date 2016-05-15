#include <cstdio>
#include <ctime>

#include "c_Time.h"

Time::Time()
{
	time_t ltime;
	time(&ltime);
	tm *n;
	n=localtime(&ltime);
	this->t.tm_sec=		n->tm_sec;		// seconds after the minute - [0,59]
	this->t.tm_min=		n->tm_min;		// minutes after the hour - [0,59]
	this->t.tm_hour=	n->tm_hour;		// hours since midnight - [0,23]
	this->t.tm_mday=	n->tm_mday;		// day of the month - [1,31]
	this->t.tm_mon=		n->tm_mon;		// months since January - [0,11]
	this->t.tm_year=	n->tm_year;		// years since 1900
	this->t.tm_wday=	n->tm_wday;		// days since Sunday - [0,6]
	this->t.tm_yday=	n->tm_yday;		// days since January 1 - [0,365]
	this->t.tm_isdst=	n->tm_isdst;	// daylight savings time flag
}
