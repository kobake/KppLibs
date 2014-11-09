#pragma once

#include "common/std/std.h"

#include <assert.h>

// -- -- -- -- グローバルなチェーン空間しか持たないチェーン構造 -- -- -- -- //
// 巡回中に任意の SimpleChain が delete されても安全であることを保証する！

template <class T>
class SimpleChain{
private:
	typedef SimpleChain<T> Me;
public:
	//コンストラクタ・デストラクタ
	SimpleChain()
	{
		if(gm_front==NULL){
			//先頭であり末尾
			gm_front = static_cast<T*>(this);
			gm_back = static_cast<T*>(this);
			this->m_prev = NULL;
			this->m_next = NULL;
		}
		else{
			//末尾と結合
			gm_back->m_next = static_cast<T*>(this);
			this->m_prev = gm_back;
			this->m_next = NULL;
			//末尾の世代交代
			gm_back = static_cast<T*>(this);
		}
	}
	virtual ~SimpleChain()
	{
		//直前処理
		if(m_prev){
			//直前との離脱
			m_prev->m_next = this->m_next;
		}
		else{
			//先頭の世代交代
			gm_front = this->m_next;
		}
		//直後処理
		if(m_next){
			//直後との離脱
			m_next->m_prev = this->m_prev;
		}
		else{
			//末尾の世代交代
			gm_back = this->m_prev;
		}
	}
	//巡回
	static T* S_ChainFront(){ return gm_front; }
	static T* S_ChainBack(){ return gm_back; }
	T* ChainNext(){ return m_next; }
	T* ChainPrev(){ return m_prev; }
private:
	T*	m_prev;
	T*	m_next;
	static T*	gm_front;
	static T*	gm_back;
};


template <class T> T* SimpleChain<T>::gm_front = NULL;
template <class T> T* SimpleChain<T>::gm_back = NULL;
