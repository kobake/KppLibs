/*
	�R���e�i��
*/

#pragma once

#include <list>
#include <algorithm> //find

//! list�ɂ�����Ƌ@�\��ǉ�������
template <class T>
class list_ex : public std::list<T>{
private:
	typedef std::list<T> Super;

public:
	// -- -- �萔 -- -- //
	static const size_t INVALID_INDEX = (size_t)-1;

public:
	// -- -- �R���X�g���N�^�E�f�X�g���N�^ -- -- //
	list_ex() : Super() {}
	list_ex(const Super& rhs) : Super(rhs) { }

	// -- -- �C���^�[�t�F�[�X -- -- //
	Super::iterator back_iterator()
	{
		Super::iterator p = end();
		return --p;
	}


};

