/*
	コンテナ類
*/

#pragma once

#include <list>
#include <algorithm> //find

//! listにちょっと機能を追加した版
template <class T>
class list_ex : public std::list<T>{
private:
	typedef std::list<T> Super;

public:
	// -- -- 定数 -- -- //
	static const size_t INVALID_INDEX = (size_t)-1;

public:
	// -- -- コンストラクタ・デストラクタ -- -- //
	list_ex() : Super() {}
	list_ex(const Super& rhs) : Super(rhs) { }

	// -- -- インターフェース -- -- //
	Super::iterator back_iterator()
	{
		Super::iterator p = end();
		return --p;
	}


};

