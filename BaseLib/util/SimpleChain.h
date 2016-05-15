#pragma once

#include "common/std/std.h"

#include <assert.h>

// -- -- -- -- �O���[�o���ȃ`�F�[����Ԃ��������Ȃ��`�F�[���\�� -- -- -- -- //
// ���񒆂ɔC�ӂ� SimpleChain �� delete ����Ă����S�ł��邱�Ƃ�ۏ؂���I

template <class T>
class SimpleChain{
private:
	typedef SimpleChain<T> Me;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	SimpleChain()
	{
		if(gm_front==NULL){
			//�擪�ł��薖��
			gm_front = static_cast<T*>(this);
			gm_back = static_cast<T*>(this);
			this->m_prev = NULL;
			this->m_next = NULL;
		}
		else{
			//�����ƌ���
			gm_back->m_next = static_cast<T*>(this);
			this->m_prev = gm_back;
			this->m_next = NULL;
			//�����̐�����
			gm_back = static_cast<T*>(this);
		}
	}
	virtual ~SimpleChain()
	{
		//���O����
		if(m_prev){
			//���O�Ƃ̗��E
			m_prev->m_next = this->m_next;
		}
		else{
			//�擪�̐�����
			gm_front = this->m_next;
		}
		//���㏈��
		if(m_next){
			//����Ƃ̗��E
			m_next->m_prev = this->m_prev;
		}
		else{
			//�����̐�����
			gm_back = this->m_prev;
		}
	}
	//����
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
