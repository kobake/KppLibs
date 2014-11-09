#pragma once

//! ヒープを用いない文字列クラス
//2007.09.23 kobake 作成。
template <class CHAR_TYPE, int N_BUFFER_COUNT>
class TStaticString{
private:
	typedef TStaticString<CHAR_TYPE,N_BUFFER_COUNT> Me;
public:
	static const int BUFFER_COUNT = N_BUFFER_COUNT;
public:
	//コンストラクタ・デストラクタ
	TStaticString(){ m_szData[0]=0; }
	TStaticString(const CHAR_TYPE* rhs){ if(!rhs) m_szData[0]=0; else wcscpy(m_szData,rhs); }

	//クラス属性
	size_t GetBufferCount() const{ return N_BUFFER_COUNT; }

	//データアクセス
	CHAR_TYPE*       GetBufferPointer()      { return m_szData; }
	const CHAR_TYPE* GetBufferPointer() const{ return m_szData; }
	const CHAR_TYPE* c_str()            const{ return m_szData; } //std::string風

	//簡易データアクセス
	operator       CHAR_TYPE*()      { return m_szData; }
	operator const CHAR_TYPE*() const{ return m_szData; }
	CHAR_TYPE At(int nIndex) const{ return m_szData[nIndex]; }

	//簡易コピー
	void Assign(const CHAR_TYPE* src){ if(!src) m_szData[0]=0; else wcscpy(m_szData,_countof(m_szData),src); }
	Me& operator = (const CHAR_TYPE* src){ Assign(src); return *this; }

	//各種メソッド
	int Length() const{ return wcslen(m_szData); }

private:
	CHAR_TYPE m_szData[N_BUFFER_COUNT];
};

#define _countof2(s) s.BUFFER_COUNT
