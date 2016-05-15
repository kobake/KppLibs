#pragma once

class SystemTime : public SYSTEMTIME{
public:
	SystemTime();
	SystemTime(FILETIME *filetime);
	void setFileTime(FILETIME *filetime);
	wchar *toString();
	int compare(SystemTime *time);
};


