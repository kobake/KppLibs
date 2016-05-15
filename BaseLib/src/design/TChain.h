#pragma once

#include <MemoryLib.h>
#include "design/TSingleton.h"
#include <map>

/*
	�A���P�[�^
*/
template <class T>
class TChainAllocator : public CUnitAllocator{
public:
	TChainAllocator();
};

/*
	�`�F�[���v�f
*/
template <class T>
class TChain{
public:
	static TChainAllocator<T>	g_allocator;

public:
	void* operator new(size_t size)		{ assert(size <= sizeof(TChain<T>) + 4); return g_allocator.Alloc(); }
	void* operator new[](size_t size)	{ return ::malloc(size); }
	void operator delete(void* p)		{ g_allocator.Free(p);	}
	void operator delete[](void* p)		{ ::free(p); }

public:
	TChain(const T& data) : m_data(data), m_prev(NULL), m_next(NULL) {}
	~TChain() { Leave(); }
	void Leave()
	{
		if(m_prev){
			m_prev->m_next = m_next;
		}
		if(m_next){
			m_next->m_prev = m_prev;
		}
		m_prev = NULL;
		m_next = NULL;
	}
	void InsertTo(TChain<T>* p)	// p �̎�O�ɑ}�� //
	{
		assert(p);
		assert(!m_prev); // ���̃��X�g�ɒǉ��ς݂̏ꍇ��NG //
		assert(!m_next); // ���̃��X�g�ɒǉ��ς݂̏ꍇ��NG //
		m_prev = p->m_prev;
		m_next = p;
		if(p->m_prev){
			p->m_prev->m_next = this;
		}
		p->m_prev = this;
	}
	void AddTo(TChain<T>* p)	// p �̒���ɑ}�� //
	{
		assert(p);
		assert(!m_prev); // ���̃��X�g�ɒǉ��ς݂̏ꍇ��NG //
		assert(!m_next); // ���̃��X�g�ɒǉ��ς݂̏ꍇ��NG //
		m_prev = p;
		m_next = p->m_next;
		if(p->m_next){
			p->m_next->m_prev = this;
		}
		p->m_next = this;
	}
	TChain<T>* Prev()	{ return m_prev; }
	TChain<T>* Next()	{ return m_next; }
	T& GetData()		{ return m_data; }
private:
	T			m_data;
	TChain<T>*	m_prev;
	TChain<T>*	m_next;
};
template <class T>
TChainAllocator<T> TChain<T>::g_allocator;

/*
	�A���P�[�^����
*/
template <class T>
TChainAllocator<T>::TChainAllocator()
: CUnitAllocator( sizeof(TChain<T>) + 4 )
{
}

/*
	�`�F�[�����X�g
*/
template <class T>
class TChainHolder{
public:
	typedef TChain<T>* iterator;
public:
	TChainHolder()
	{
		m_front = m_back = NULL;
		m_size = 0;
	}
	~TChainHolder()
	{
		clear();
	}
	void		clear()
	{
		while(m_front){
			pop_front();
		}
		m_size = 0;
	}
	TChain<T>*	begin()			{ return m_front; }
	TChain<T>*	end()			{ return NULL; }
	TChain<T>*	front()			{ return m_front; }
	TChain<T>*	back()			{ return m_back; }
	bool		empty() const	{ return m_front == NULL; }
	size_t		size() const	{ return m_size; }
	void push_back(const T& t)
	{
		TChain<T>* p = new TChain<T>(t);
		if(!m_front){
			m_front = m_back = p;
		}
		else{
			p->AddTo(m_back);
			m_back = p;
		}
		m_size++;
	}
	void pop_back()
	{
		if(m_back){
			TChain<T>* pLeave = m_back;
			m_back = m_back->Prev();
			pLeave->Leave();
			delete pLeave;
			if(!m_back){
				m_front = NULL;
			}
			m_size--;
		}
	}
	void push_front(const T& t)
	{
		TChain<T>* p = new TChain<T>(t);
		if(!m_front){
			m_front = m_back = p;
		}
		else{
			p->InsertTo(m_front);
			m_front = p;
		}
		m_size++;
	}
	void pop_front()
	{
		if(m_front){
			TChain<T>* pLeave = m_front;
			m_front = m_front->Next();
			pLeave->Leave();
			delete pLeave;
			if(!m_front){
				m_back = NULL;
			}
			m_size--;
		}
	}
private:
	TChain<T>*	m_front;
	TChain<T>*	m_back;
	size_t		m_size;
};

template <class T>
class TChainHolder2 : public TChainHolder<T>{
private:
	// front�n�֐��𕕂��� //
	void push_front(const T& t);
	void pop_front();

public:
	void clear()
	{
		TChainHolder<T>::clear();
		m_mapPoints.clear();
	}
	void push_back(const T& t)
	{
		TChainHolder<T>::push_back(t);
		int iLastIndex = (int)(size() - 1);
		if(iLastIndex % 100 == 0){
			m_mapPoints[iLastIndex] = back();
		}
	}
	void pop_back()
	{
		TChainHolder<T>::pop_back();
		int iDeletedIndex = (int)size();
		if(iDeletedIndex % 100 == 0){
			m_mapPoints.erase(iDeletedIndex);
		}
	}
	TChain<T>*	at(int iIndex)
	{
		// ���O�̋�؂�ڂ�T�� //
		int iDiv = iIndex / 100 * 100;
		std::map<int, TChain<T>*>::const_iterator itr = m_mapPoints.find(iDiv);
		if(itr == m_mapPoints.end()){
			return NULL;
		}
		TChain<T>* p = itr->second;

		// ���ۂ̃C���f�b�N�X�܂Ői�� //
		int n = iIndex - iDiv;
		for(int i = 0; i < n && p; i++){
			p = p->Next();
		}
		return p;
	}

private:
	// 100�����ɏꏊ��ۑ� //
	std::map<int, TChain<T>*>	m_mapPoints;
};
