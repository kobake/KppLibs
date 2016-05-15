#pragma once

//base‚ÖˆÚs—\’è
template <class T> class THeap{
public:
	THeap()
	{
		m_p = new T;
	}
	THeap(const THeap<T>& rhs)
	{
		m_p = new T;
		*m_p = *rhs.m_p;
	}
	THeap(const T& rhs)
	{
		m_p = new T;
		*m_p = rhs;
	}
	~THeap()
	{
		delete m_p;
	}
	T* operator->(){ return m_p; }
	const T* operator->() const{ return m_p; }
	operator T*(){ return m_p; }
	operator const T*() const{ return m_p; }
private:
	T* m_p;
};
