#include "_required.h"
#include "TimeWatch.h"
#include <windows.h>
#include <mmsystem.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

TimeWatch::TimeWatch()
{
	timeGetTime();
	running=false;
	start_time=stop_time=total_time=0;
}
TimeWatch::~TimeWatch()
{
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void TimeWatch::start()
{
	if(!running){
		start_time=timeGetTime();
		running=true;
	}
}
void TimeWatch::stop()
{
	if(running){
		stop_time=timeGetTime();
		total_time+=stop_time-start_time;
		running=false;
	}
}
void TimeWatch::reset()
{
	total_time=0;
	running=false;
}

DWORD TimeWatch::get()
{
	if(running)
		return total_time+timeGetTime()-start_time;
	else
		return total_time;
}
