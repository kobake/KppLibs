#pragma once

#include <queue>
#include "CCriticalSection.h"

//!スレッドセーフなキュー
template <class T>
class CThreadQueue{
public:
	void Push(const T& t)	{ CRITICAL_ENTER(m_cCritical); m_que.push(t); }
	void Pop(T* t)			{ CRITICAL_ENTER(m_cCritical); *t = m_que.front(); m_que.pop(); }
	bool IsEmpty() const	{ CRITICAL_ENTER(m_cCritical); return m_que.empty(); }

private:
	std::queue<T>				m_que;
	mutable CCriticalSection	m_cCritical;
};
