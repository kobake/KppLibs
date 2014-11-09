#include "_required.h"
#include "CPersonalTime.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �ݒ�                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!�����Ԃ��u�t���[�����v�P�ʂŐݒ�
void CPersonalTime::SetTotalFrame(uint frame)
{
	m_milli=frame*1000/60;
}

//!�����Ԃ��u�~���b�v�P�ʂŐݒ�
void CPersonalTime::SetTotalMilliSec(uint milli_sec)
{
	m_milli=milli_sec;
}

//!�����Ԃ��u�b�v�P�ʂŐݒ�
void CPersonalTime::SetTotalSec(uint sec)
{
	m_milli=sec*1000;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          �\���p                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!���ԕ\���́u���ԁv������Ԃ��B0�`��
uint CPersonalTime::GetHour() const
{
	return GetTotalHour();
}

//!���ԕ\���́u���v������Ԃ��B0�`59
uint CPersonalTime::GetMinute() const
{
	return GetTotalMinute()%60;
}

//!���ԕ\���́u�b�v������Ԃ��B0�`59
uint CPersonalTime::GetSec() const
{
	return GetTotalSec()%60;
}

//!���ԕ\���́u�b�v�ȉ���2��������Ԃ��B0�`99
uint CPersonalTime::GetComma() const
{
	return (GetTotalMilliSec()/10)%100;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �P��                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!�����Ԃ��u���ԁv�P�ʂŕԂ��B0�`��
uint CPersonalTime::GetTotalHour() const
{
	return GetTotalMinute()/60;
}

//!�����Ԃ��u���v�P�ʂŕԂ��B0�`��
uint CPersonalTime::GetTotalMinute() const
{
	return GetTotalSec()/60;
}

//!�����Ԃ��u�b�v�P�ʂŕԂ��B0�`��
uint CPersonalTime::GetTotalSec() const
{
	return GetTotalMilliSec()/1000;
}

//!�����Ԃ��u�~���b�v�P�ʂŕԂ��B0�`��
uint CPersonalTime::GetTotalMilliSec() const
{
	return m_milli;
}

//<�����Ԃ��u�t���[�����v�P�ʂŕԂ��B
uint CPersonalTime::GetTotalFrame() const
{
	return m_milli*60/1000; //60fps��z��
}
