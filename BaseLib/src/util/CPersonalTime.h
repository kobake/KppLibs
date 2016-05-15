#pragma once

class CInputStream;
class COutputStream;


class CPersonalTime{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CPersonalTime() : m_milli(0) { }
	CPersonalTime(const CPersonalTime& rhs) : m_milli(rhs.m_milli) { }
	//�ݒ�
	void SetTotalFrame(uint frame);        //!<�����Ԃ��u�t���[�����v�P�ʂŐݒ�
	void SetTotalMilliSec(uint milli_sec); //!<�����Ԃ��u�~���b�v�P�ʂŐݒ�
	void SetTotalSec(uint sec);            //!<�����Ԃ��u�b�v�P�ʂŐݒ�
	//���Z�q
	bool operator <  (const CPersonalTime& rhs) const{ return m_milli <  rhs.m_milli; }
	bool operator <= (const CPersonalTime& rhs) const{ return m_milli <= rhs.m_milli; }
	bool operator >  (const CPersonalTime& rhs) const{ return m_milli >  rhs.m_milli; }
	bool operator >= (const CPersonalTime& rhs) const{ return m_milli >= rhs.m_milli; }
	bool operator == (const CPersonalTime& rhs) const{ return m_milli == rhs.m_milli; }
	bool operator != (const CPersonalTime& rhs) const{ return m_milli != rhs.m_milli; }
	CPersonalTime& operator = (const CPersonalTime& rhs){ m_milli = rhs.m_milli; return *this; }
	//�\���p
	uint GetHour() const;   //!<���ԕ\���́u���ԁv������Ԃ��B0�`��
	uint GetMinute() const; //!<���ԕ\���́u���v������Ԃ��B0�`59
	uint GetSec() const;    //!<���ԕ\���́u�b�v������Ԃ��B0�`59
	uint GetComma() const;  //!<���ԕ\���́u�b�v�ȉ���2��������Ԃ��B0�`99
	//�P��
	uint GetTotalHour() const;     //!<�����Ԃ��u���ԁv�P�ʂŕԂ��B0�`��
	uint GetTotalMinute() const;   //!<�����Ԃ��u���v�P�ʂŕԂ��B0�`��
	uint GetTotalSec() const;      //!<�����Ԃ��u�b�v�P�ʂŕԂ��B0�`��
	uint GetTotalMilliSec() const; //!<�����Ԃ��u�~���b�v�P�ʂŕԂ��B0�`��
	uint GetTotalFrame() const;    //!<�����Ԃ��u�t���[�����v�P�ʂŕԂ��B

private:
	uint m_milli;
};

