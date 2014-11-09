#pragma once

#include <assert.h>

// -- -- -- -- ���g�����[�g�����˂� ChainEx -- -- -- -- //
// ���񒆂ɔC�ӂ� ChainEx �� delete ����Ă����S�ł��邱�Ƃ�ۏ؂���I

template <class T> class ChainSpaceEx{
public:
	//����_�ۑ��N���X
	class ChainCur{
	public:
		ChainCur()
		{
			for(int i=0;i<32;i++)table[i]=NULL;
			cur=&table[0];
		}
		void push(T* t)
		{
			*cur++=t;
		}
		T* pop()
		{
			return *--cur;
		}
		T* top() const
		{
			return *(cur-1);
		}
		size_t size() const
		{
			return (size_t)(cur-table);
		}
		void check(T* t,T* next)
		{
			for(size_t i=0;i<size();i++){
				if(table[i]==t){
					table[i]=next;
					break;
				}
			}
		}
	private:
		T** cur;
		T* table[32]; //32�i�K�̍ċA�ɑΉ�
	};
	static ChainCur s_next_table;
	static ChainCur s_prev_table;
	static ChainCur s_current_table;
	static ChainCur s_holder_table;

	class ChainHolder{
	public:
		//�R���X�g���N�^�E�f�X�g���N�^
		ChainHolder()
		{
			s_chain_top=NULL;
			s_chain_end=NULL;
			s_chain_count=NULL;
		}
		virtual ~ChainHolder()
		{
			//���X�g����ł��邱�Ƃ��m�F
			assert(s_chain_top==NULL);
		}
	public:
		void chainKill()
		{
			//�q���폜
			T* t=s_chain_end;
			T* d;
			while(t){
				d=t;
				t=t->chain_prev;
				d->chainKill();
			}
			//�������폜
			delete this;
		}
		//���X�g�Ǘ�
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
		//����p�̌^ (���ǂ���̏���ɂ����Ă��AT�͎�����proc�̒��ł���delete����Ȃ����Ƃ�O��Ƃ���)
		typedef void (*EnumProc)(T*);
		//����
		void chainEnum(EnumProc proc,bool r=false) //r �c �ċA
		{
			T* t=s_chain_top;
			s_holder_table.push(static_cast<T*>(this)); //this��ۑ�
			while(t){
				s_next_table.push(t->chain_next);    //����ۑ�
				s_current_table.push(t);             //������ۑ�
				proc(t);                             //���������s
				if(s_holder_table.top()==NULL)break; //this���`�F�b�N
				//���ɐi�ޑO�Ɏ����̎q������
				if(s_current_table.pop()==t && r){   //�������܂����݂���Ȃ�
					t->chainEnum(proc,r);
				}
				t=s_next_table.pop();                //����
			}
			s_holder_table.pop();
		}
		//�t����
		void chainEnumR(EnumProc proc,bool r=false) //r �c �ċA
		{
			T* t=s_chain_end;
			s_holder_table.push(static_cast<T*>(this)); //this��ۑ�
			while(t){
				s_prev_table.push(t->chain_prev);    //�O��ۑ�
				s_current_table.push(t);             //������ۑ�
				//�܂������̎q������
				if(r){
					t->chainEnumR(proc,r);
				}
				if(s_current_table.pop()==t){        //�������܂����݂���Ȃ�
					proc(t);                         //���������s
				}
				if(s_holder_table.top()==NULL)break; //this���`�F�b�N
				t=s_prev_table.pop();                //�O��
			}
			s_holder_table.pop();
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
	protected:
		T* s_chain_top;
		T* s_chain_end;
		int s_chain_count;
	};

	class ChainEx : public ChainHolder{
	public:
		friend class ChainHolder;
	public:
		//�R���X�g���N�^�E�f�X�g���N�^
		ChainEx(ChainHolder* _parent=NULL)
		: parent(_parent)
		{
			if(parent==NULL)parent=chainRoot();
			parent->attach(static_cast<T*>(this));
			chain_member=1;
		}
		virtual ~ChainEx()
		{
			chainDetach();
		}
	public:
		//����
		void chainDetach()
		{
			if(chain_member){
				s_next_table.check(static_cast<T*>(this),chain_next);
				s_prev_table.check(static_cast<T*>(this),chain_prev);
				s_current_table.check(static_cast<T*>(this),NULL);
				s_holder_table.check(static_cast<T*>(this),NULL);
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

};
template <class T> typename ChainSpaceEx<T>::ChainHolder* ChainSpaceEx<T>::ChainEx::default_parent=NULL;
template <class T> typename ChainSpaceEx<T>::ChainCur ChainSpaceEx<T>::s_next_table;
template <class T> typename ChainSpaceEx<T>::ChainCur ChainSpaceEx<T>::s_prev_table;
template <class T> typename ChainSpaceEx<T>::ChainCur ChainSpaceEx<T>::s_current_table;
template <class T> typename ChainSpaceEx<T>::ChainCur ChainSpaceEx<T>::s_holder_table;

