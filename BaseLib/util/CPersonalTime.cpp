#include "_required.h"
#include "CPersonalTime.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           設定                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!総時間を「フレーム数」単位で設定
void CPersonalTime::SetTotalFrame(uint frame)
{
	m_milli=frame*1000/60;
}

//!総時間を「ミリ秒」単位で設定
void CPersonalTime::SetTotalMilliSec(uint milli_sec)
{
	m_milli=milli_sec;
}

//!総時間を「秒」単位で設定
void CPersonalTime::SetTotalSec(uint sec)
{
	m_milli=sec*1000;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          表示用                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!時間表示の「時間」部分を返す。0～∞
uint CPersonalTime::GetHour() const
{
	return GetTotalHour();
}

//!時間表示の「分」部分を返す。0～59
uint CPersonalTime::GetMinute() const
{
	return GetTotalMinute()%60;
}

//!時間表示の「秒」部分を返す。0～59
uint CPersonalTime::GetSec() const
{
	return GetTotalSec()%60;
}

//!時間表示の「秒」以下の2桁部分を返す。0～99
uint CPersonalTime::GetComma() const
{
	return (GetTotalMilliSec()/10)%100;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           単位                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!総時間を「時間」単位で返す。0～∞
uint CPersonalTime::GetTotalHour() const
{
	return GetTotalMinute()/60;
}

//!総時間を「分」単位で返す。0～∞
uint CPersonalTime::GetTotalMinute() const
{
	return GetTotalSec()/60;
}

//!総時間を「秒」単位で返す。0～∞
uint CPersonalTime::GetTotalSec() const
{
	return GetTotalMilliSec()/1000;
}

//!総時間を「ミリ秒」単位で返す。0～∞
uint CPersonalTime::GetTotalMilliSec() const
{
	return m_milli;
}

//<総時間を「フレーム数」単位で返す。
uint CPersonalTime::GetTotalFrame() const
{
	return m_milli*60/1000; //60fpsを想定
}
