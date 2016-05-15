#pragma once

#include "sdk/libfwd_win.h" //DWORD

class TimeWatch{
protected:
	bool running;
	DWORD start_time;
	DWORD stop_time;
	DWORD total_time;
public:
	TimeWatch();
	virtual ~TimeWatch();
	void start();
	void stop();
	void reset();
	DWORD get();
};


