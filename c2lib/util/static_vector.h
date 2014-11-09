#pragma once

#include "debug/debug.h"

namespace util{

//##assert系は今後ぜんぶポリシーにする
template <class T, int SIZE> class static_vector{
public:
	typedef static_vector<T, SIZE> Me;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T& reference;
	typedef const T& const_reference;
	static int limit(){ return SIZE; }
public:
	static_vector()
	{
		size_=0;
	}
	template <int SIZE2> static_vector(const static_vector<T, SIZE2>& rhs)
	{
		size_=0;
		operator=(rhs);
	}
	template <int SIZE2> static_vector<T, SIZE>& operator=(const static_vector<T, SIZE2>& rhs)
	{
		assertmsg(SIZE>=rhs.size(), "asign failed");
		for(int i=0;i<rhs.size();i++){
			data[i]=rhs[i];
		}
		size_=rhs.size();
		return *this;
	}
	//アクセサ
	reference operator[](int i)
	{
		assert(i>=0 && i<size_);
		return data[i];
	}
	const_reference operator[](int i) const
	{
		assert(i>=0 && i<size_);
		return data[i];
	}
	reference at(int i)
	{
		assert(i>=0 && i<size_);
		return data[i];
	}
	const_reference at(int i) const
	{
		assert(i>=0 && i<size_);
		return data[i];
	}
	//便利アクセサ
	reference front()
	{
		assert(size_>0);
		return data[0];
	}
	const_reference front() const
	{
		assert(size_>0);
		return data[0];
	}
	reference back()
	{
		assert(size_>0);
		return data[size_-1];
	}
	const_reference back() const
	{
		assert(size_>0);
		return data[size_-1];
	}
	//イタレータ
	iterator begin()
	{
		return &data[0];
	}
	const_iterator begin() const
	{
		return &data[0];
	}
	iterator end()
	{
		return &data[size_];
	}
	const_iterator end() const
	{
		return &data[size_];
	}
	//状態
	int size() const
	{
		return size_;
	}
	int remainder_size() const //!<空き容量
	{
		return SIZE-size_;
	}
	bool empty() const
	{
		return size_==0;
	}
	bool isfull() const
	{
		return size_>=SIZE;
	}
	//変更
	void clear()
	{
		size_=0;
	}
	void resize(int new_size)
	{
		size_=new_size;
		assert(size_>=0 && size_<=SIZE);
	}
	void push_back(const T& t)
	{
		size_++;
		assert(size_<=SIZE);
		data[size_-1]=t;
	}
	void push_front(const T& t)
	{
		size_++;
		assert(size_<=SIZE);
		//右へずらす
		for(int i=size_-1;i>=1;i--){
			data[i]=data[i-1];
		}
		data[0]=t;
	}
	void pop_back()
	{
		resize(size_-1);
	}
	void pop_front()
	{
		//左へずらす
		for(int i=0;i<size_-1;i++){
			data[i]=data[i+1];
		}
		resize(size_-1);
	}
	void erase(int index)
	{
		erase(index,index+1);
	}
	void erase(int first,int last)
	{
		assert(first<last);
		assert(first>=0);
		assert(last<=size_);
		int n=size_-last;
		for(int i=0;i<n;i++){
			data[first+i]=data[last+i];
		}
		size_-=last-first;
	}
	void remove_index(int index)
	{
		for(int i=index;i<size_-1;i++){
			data[i]=data[i+1];
		}
		resize(size_-1);
	}
	int remove_value(const T& t)
	{
		int cnt=0;
		int first=-1,last=-1;
		for(int i=0;i<size_;i++){
			if(data[i]==t){
				if(first==-1){ first=i; last=i+1; }
				else{ last=i+1; }
			}
			else if(first!=-1){
				//削除実行
				erase(first,last);
				cnt+=last-first;
				//次の位置へ
				i=first;
				//first,lastリセット
				first=last=-1;
			}
		}
		if(first!=-1){
			//最後の削除実行
			erase(first,last);
			cnt+=last-first;
		}
		return cnt;
	}
	template <class Pred> int remove_if(Pred pr)
	{
		int cnt=0;
		int first=-1,last=-1;
		for(int i=0;i<size_;i++){
			if(pr(data[i])){
				if(first==-1){ first=i; last=i+1; }
				else{ last=i+1; }
			}
			else if(first!=-1){
				//削除実行
				erase(first,last);
				cnt+=last-first;
				//次の位置へ
				i=first;
				//first,lastリセット
				first=last=-1;
			}
		}
		if(first!=-1){
			//最後の削除実行
			erase(first,last);
			cnt+=last-first;
		}
		return cnt;
	}
	//検索
	int find(const T& t) const
	{
		for(int i=0;i<size_;i++){
			if(data[i]==t)return i;
		}
		return -1;
	}
	//便利機能
	bool push_back_unique(const T& t)
	{
		if(find(t)==-1){
			push_back(t);
			return true;
		}else{
			return false;
		}
	}
private:
	T data[SIZE];
	int size_;
};




} //namespace util

