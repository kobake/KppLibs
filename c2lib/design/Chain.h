#pragma once

#include "common/std/std.h"

#include <assert.h>

// -- -- -- -- 任意のルートを持つ Chain -- -- -- -- //
// 巡回中に任意の Chain が delete されても安全であることを保証する！

template <class T> class ChainSpace{
public:
	class ChainHolder{
	public:
		//コンストラクタ・デストラクタ
		ChainHolder()
		{
			s_chain_top=NULL;
			s_chain_end=NULL;
			s_chain_count=NULL;
		}
		virtual ~ChainHolder()
		{
			//リストが空であることを確認
			assert(s_chain_top==NULL);
		}
		//リスト管理
		void attach(T* p)
		{
			if(s_chain_end)s_chain_end->chain_next=p;
			else s_chain_top=p;
			p->chain_prev=s_chain_end;
			p->chain_next=NULL;
			s_chain_end=p;
			s_chain_count++;
		}
		void detach(T* p)
		{
			if(p->chain_prev)p->chain_prev->chain_next=p->chain_next;
			else s_chain_top=p->chain_next;
			if(p->chain_next)p->chain_next->chain_prev=p->chain_prev;
			else s_chain_end=p->chain_prev;
			p->chain_prev=NULL;
			p->chain_next=NULL;
			s_chain_count--;
		}
		//巡回
		typedef int (*EnumProc)(T*);
		void chainEnum(EnumProc proc)
		{
			T* t=s_chain_top;
			while(t){
				s_cur_next=t->chain_next;
				proc(t);
				t=s_cur_next;
			}
			s_cur_next=NULL;
		}
		bool chainExist(T* _t)
		{
			T* t=s_chain_top;
			while(t){
				if(t==_t)return true;
				t=t->chain_next;
			}
			return false;
		}
		int chainCount()
		{
			return s_chain_count;
		}
	private:
		T* s_chain_top;
		T* s_chain_end;
		int s_chain_count;
	};

	class Chain{
	public:
		friend class ChainHolder;
	public:
		//コンストラクタ・デストラクタ
		Chain(ChainHolder* _parent=NULL)
		: parent(_parent)
		{
			if(parent==NULL)parent=chainRoot();
			parent->attach(static_cast<T*>(this));
			chain_member=1;
		}
		virtual ~Chain()
		{
			chainDetach();
		}
	public:
		//特殊
		void chainDetach()
		{
			if(chain_member){
				if(s_cur_next==static_cast<T*>(this)){
					s_cur_next=chain_next;
				}
				parent->detach(static_cast<T*>(this));
				chain_member=0;
			}
		}
	private:
		ChainHolder* parent;
		T* chain_prev;
		T* chain_next;
		int chain_member;


	// -- -- static -- -- //
	public:
		static ChainHolder* chainRoot()
		{
			if(default_parent==NULL){
				default_parent=new ChainHolder();
			}
			return default_parent;
		}
	private:
		static ChainHolder* default_parent;
	};

	static T* s_cur_next;

};
template <class T> typename ChainSpace<T>::ChainHolder* ChainSpace<T>::Chain::default_parent=NULL;
template <class T> T* ChainSpace<T>::s_cur_next=NULL;
