/*
	�R���e�i��

	2007.11.27 kobake �쐬
*/

#pragma once

#include <vector>
#include <algorithm> //find

//! vector�ɂ�����Ƌ@�\��ǉ�������
template <class T>
class vector_ex : public std::vector<T>{
private:
	typedef std::vector<T> SUPER;

public:
	// -- -- �萔 -- -- //
	static const size_t INVALID_INDEX = (size_t)-1;

public:
	// -- -- �R���X�g���N�^�E�f�X�g���N�^ -- -- //
	vector_ex() : SUPER() {}
	vector_ex(const SUPER& rhs) : SUPER(rhs) { }

	// -- -- �C���^�[�t�F�[�X -- -- //
	//!�v�f��T���B�������炻�̃C���f�b�N�X��Ԃ��B������Ȃ�������INVALID_INDEX��Ԃ��B
	size_t index_of(const T& t) const
	{
		const_iterator p = find(begin(),end(),t);
		if(p!=end()){
			return p-begin();
		}
		else{
			return INVALID_INDEX;
		}
	}

	//!�v�f��T���B�������true�B
	bool exists(const T& t) const
	{
		return index_of(t)!=INVALID_INDEX;
	}

	//!�v�f��ǉ��B�������d�������v�f�͒e���B
	void push_back_unique(const T& t)
	{
		if(!exists(t))
			push_back(t);
	}

	//!�l����v����v�f��1�폜
	bool erase_value(const T& t)
	{
		size_t i = index_of(t);
		if(i==INVALID_INDEX)return false;
		erase(begin()+i);
		return true;
	}

};

