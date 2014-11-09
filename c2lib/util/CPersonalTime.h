#pragma once

class CInputStream;
class COutputStream;


class CPersonalTime{
public:
	//コンストラクタ・デストラクタ
	CPersonalTime() : m_milli(0) { }
	CPersonalTime(const CPersonalTime& rhs) : m_milli(rhs.m_milli) { }
	//設定
	void SetTotalFrame(uint frame);        //!<総時間を「フレーム数」単位で設定
	void SetTotalMilliSec(uint milli_sec); //!<総時間を「ミリ秒」単位で設定
	void SetTotalSec(uint sec);            //!<総時間を「秒」単位で設定
	//演算子
	bool operator <  (const CPersonalTime& rhs) const{ return m_milli <  rhs.m_milli; }
	bool operator <= (const CPersonalTime& rhs) const{ return m_milli <= rhs.m_milli; }
	bool operator >  (const CPersonalTime& rhs) const{ return m_milli >  rhs.m_milli; }
	bool operator >= (const CPersonalTime& rhs) const{ return m_milli >= rhs.m_milli; }
	bool operator == (const CPersonalTime& rhs) const{ return m_milli == rhs.m_milli; }
	bool operator != (const CPersonalTime& rhs) const{ return m_milli != rhs.m_milli; }
	CPersonalTime& operator = (const CPersonalTime& rhs){ m_milli = rhs.m_milli; return *this; }
	//表示用
	uint GetHour() const;   //!<時間表示の「時間」部分を返す。0～∞
	uint GetMinute() const; //!<時間表示の「分」部分を返す。0～59
	uint GetSec() const;    //!<時間表示の「秒」部分を返す。0～59
	uint GetComma() const;  //!<時間表示の「秒」以下の2桁部分を返す。0～99
	//単位
	uint GetTotalHour() const;     //!<総時間を「時間」単位で返す。0～∞
	uint GetTotalMinute() const;   //!<総時間を「分」単位で返す。0～∞
	uint GetTotalSec() const;      //!<総時間を「秒」単位で返す。0～∞
	uint GetTotalMilliSec() const; //!<総時間を「ミリ秒」単位で返す。0～∞
	uint GetTotalFrame() const;    //!<総時間を「フレーム数」単位で返す。

private:
	uint m_milli;
};

