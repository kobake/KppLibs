#pragma once

#include <vector>

template <class T>
class myvector : public std::vector<T>{
public:
	myvector<T>() : std::vector<T>() {}
	explicit myvector<T>(const std::vector<T>& v);
	bool contains(const T& t) const;
	T join(const T& t) const;
};

template <class T>
myvector<T>::myvector(const std::vector<T>& v)
{
	for(int i = 0; i < (int)this->size(); i++){
		const T& t = v[i];
		this->push_back(t);
	}
}

template <class T>
bool myvector<T>::contains(const T& t) const
{
	for(int i = 0; i < (int)this->size(); i++){
		const T& t2 = (*this)[i];
		if(t == t2){
			return true;
		}
	}
	return false;
}

template <class T>
T myvector<T>::join(const T& glue) const
{
	T ret;
	for(int i = 0; i < (int)this->size(); i++){
		if(i > 0){
			ret += glue;
		}
		const T& t = (*this)[i];
		ret += t;
	}
	return ret;
}
