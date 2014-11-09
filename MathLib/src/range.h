#pragma once

#ifdef min
	#undef min
	#undef max
#endif

template <class T> T adjust(T t, T min_value, T max_value)
{
	if(t<min_value)return min_value;
	if(t>max_value)return max_value;
	return t;
}

template <class T> class range{ //###ASSERT_POLICY=DEFAULT_ASSERT,EXIT_ASSERT,EXCEPTION_ASSERT,NO_ASSERT
public:
	class invalid_range{};
public:
	range(T _min_value, T _max_value)
	: min_value(_min_value), max_value(_max_value)
	{
	}
	range()
	{
		min_value=numeric_limits<T>::min();
		max_value=numeric_limits<T>::max();
	}
	T Adjust(T t) const
	{
		return ::adjust<T>(t, min_value, max_value);
	}
	//属性
	T GetMin() const{ return min_value; }
	T GetMax() const{ return max_value; }
	void SetMin(T t){ min_value=t; _check(); }
	void SetMax(T t){ max_value=t; _check(); }
protected:
	void _check() //###本当はASSERT_POLICYに基づく
	{
		if(min_value>max_value)throw invalid_range();
	}
private:
	T min_value;
	T max_value;
};

#include <limits>
using namespace std;

template <class T> class range_value{
public:
	typedef range_value<T> Me;
public:
	//コンストラクタ・デストラクタ
	range_value()
	{
		m_value=T();
	}
	range_value(T _value, const range<T>& _range)
	{
		reset(_value, _range);
	}
	void reset(T _value, const range<T>& _range)
	{
		m_value=_value;
		m_range=_range;
		_adjust();
	}
	//型変換
	operator T() const
	{
		return m_value;
	}
	//演算子
	Me& operator = (T t)
	{
		m_value=m_range.Adjust(t);
		return *this;
	}
	Me& operator += (T t)
	{
		m_value=m_range.Adjust(m_value+t);
		return *this;
	}
	Me& operator -= (T t)
	{
		m_value=m_range.Adjust(m_value-t);
		return *this;
	}
	Me& operator ++ ()
	{
		return (*this)+=1;
	}
	Me operator ++ (int)
	{
		Me ret=*this;
		(*this)+=1;
		return ret;
	}
	Me& operator -- ()
	{
		return (*this)-=1;
	}
	Me operator -- (int)
	{
		Me ret=*this;
		(*this)-=1;
		return ret;
	}
	//属性
	const range<T>& getRange() const{ return m_range; }
	void SetRange(const range<T>& _range)
	{
		m_range=_range;
		_Adjust();
	}
protected:
	void _Adjust()
	{
		m_value=m_range.Adjust(m_value);
	}
private:
	T			m_value;
	range<T>	m_range;
};




//! FROM <= X <= TO を満たす X を表現する。
template <int FROM, int TO>
class TRangedValue{
public:
	typedef TRangedValue<FROM,TO> Me;
public:
	//コンストラクタ・デストラクタ
	TRangedValue()
	{
		m_value=0;
		_Adjust();
	}
	TRangedValue(int value)
	{
		m_value = value;
		_Adjust();
	}
	//型変換
	operator int() const{ return m_value; }
	int Value() const{ return m_value; }
	//演算子
	Me& operator =  (int t)	{ _Set(t); return *this; }
	Me& operator += (int t)	{ _Set(m_value+t); return *this; }
	Me& operator -= (int t)	{ _Set(m_value-t); return *this; }
	Me& operator ++ ()		{ return (*this)+=1; }
	Me  operator ++ (int)	{ Me ret=*this; (*this)+=1; return ret; }
	Me& operator -- ()		{ return (*this)-=1; }
	Me  operator -- (int)	{ Me ret=*this; (*this)-=1; return ret; }
protected:
	void _Adjust()
	{
		m_value = ::adjust(m_value, FROM, TO);
	}
	void _Set(int n)
	{
		m_value = ::adjust(n, FROM, TO);
	}
private:
	int	m_value;
};

/*!
	FROM <= X <= TO
	FROM <= Y <= TO
	X <= Y
	を満たす
	X ～ Y を表現する。
*/
template <int FROM, int TO>
class TRangedRange{
public:
	//型
	typedef TRangedValue<FROM,TO> ValueType;

public:
	//コンストラクタ・デストラクタ
	TRangedRange()
	{
		Set(FROM,TO);
	}
	TRangedRange(int from, int to)
	{
		Set(from,to);
	}
	//設定
	void SetFrom(int n){ m_from = n; _Adjust(); }
	void SetTo(int n){ m_to = n; _Adjust(); }
	void Set(int from, int to){ m_from = from; m_to = to; _Adjust(); }
	//取得
	const ValueType& GetFrom() const{ return m_from; }
	const ValueType& GetTo() const{ return m_to; }

protected:
	//調整
	void _Adjust()
	{
		if(m_from>m_to)m_to=m_from; //####反転するかfromに合わせるかtoに合わせるか迷うところだが…
	}

private:
	ValueType	m_from;
	ValueType	m_to;
};

