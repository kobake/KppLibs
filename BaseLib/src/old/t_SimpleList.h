#pragma once

#define SIMPLE_LIST_UNIT_SIZE 16

#include "type.h"

template <class T> class SimpleList;

template <class T> T &SimpleList_errorValue()
{
	static T ret;
	static bool init=false;
	if(!init){
		init=true;
		memset(&ret,0,sizeof(T));
	}
	return ret;
}

template <class T> void SimpleList_newValue(T &dst)
{
}

template <class T> void SimpleList_copyValue(T &dst,T &src)
{
	dst=src;
}

template <class T> void SimpleList_deleteValue(T &dst)
{
}

template <class T> bool SimpleList_equalValue(T &left,T &right)
{
	return (left==right);
}

//Å¶TÇÕäÓñ{å^Ç©ç\ë¢ëÃ
template <class T> class SimpleList{
protected:
	T *list;
	int nlist;
public:
	SimpleList();
	SimpleList(SimpleList<T> &right);
	SimpleList<T> *clone();
	virtual ~SimpleList();
	//
	int size();
	bool put(int index,T value);
	bool remove(int index);
	bool remove(T value,int max_count);
	bool set(int index,T value);
	T &get(int index);
	bool swap(int index1,int index2);
	void clear();
	void sort(COMPPROC comp);
	bool exist(const T& value);
	int indexOf(const T& value);
	//
	T *getListInstance();
};

template <class T> T *SimpleList<T>::getListInstance()
{
	return list;
}

template <class T> SimpleList<T>::SimpleList()
{
	list=NULL;
	nlist=0;
}

template <class T> SimpleList<T>::SimpleList(SimpleList<T> &right)
{
	nlist=right.nlist;
	list=(T*)calloc(nlist,sizeof(T));
	for(int i=0;i<nlist;i++){
		SimpleList_copyValue<T>(list[i],right.list[i]);
	}
}

template <class T> SimpleList<T> *SimpleList<T>::clone()
{
	return new SimpleList<T>(*this);
}

template <class T> SimpleList<T>::~SimpleList()
{
	clear();
}

template <class T> int SimpleList<T>::size()
{
	return nlist;
}

template <class T> bool SimpleList<T>::put(int index,T value)
{
	if(index==-1)index=nlist;
	if(index<0 || index>nlist)return false;
	if(nlist%SIMPLE_LIST_UNIT_SIZE==0)list=(T*)realloc(list,sizeof(T)*(nlist+SIMPLE_LIST_UNIT_SIZE));
	nlist++;
	memmove(&list[index+1],&list[index],sizeof(T)*(nlist-index-1));
	SimpleList_newValue(list[index]);
	SimpleList_copyValue(list[index],value);
	return true;
}

template <class T> bool SimpleList<T>::remove(int index)
{
	if(index==-1)index=nlist-1;
	if(index<0 || index>nlist-1)return false;
	SimpleList_deleteValue(list[index]);
	memmove(&list[index],&list[index+1],sizeof(T)*(nlist-index-1));
	nlist--;
	if(nlist%SIMPLE_LIST_UNIT_SIZE==0)list=(T*)realloc(list,sizeof(T)*nlist);
	return true;
}

template <class T> bool SimpleList<T>::remove(T value,int max_count)
{
	int i,found;
	if(max_count==-1)max_count=nlist;
	//
	found=0;
	if(max_count>0)for(i=0;i<nlist;i++){
		if(SimpleList_equalValue<T>(list[i],value)){
			memmove(&list[i],&list[i+1],sizeof(T)*(nlist-i-1));
			nlist--;
			i--;
			found++;
			max_count--; if(max_count<=0)break;
		}
	}
	return found>0;
}

template <class T> bool SimpleList<T>::set(int index,T value)
{
	if(index==-1)index=nlist-1;
	if(index<0 || index>nlist-1)return false;
	SimpleList_copyValue(list[index],value);
	return true;
}

template <class T> T &SimpleList<T>::get(int index)
{
	if(index==-1)index=nlist-1;
	if(index<0 || index>nlist-1)return SimpleList_errorValue<T>();
	return list[index];
}

template <class T> bool swap(int index1,int index2)
{
	if(index1==-1)index1=nlist-1;
	if(index2==-1)index2=nlist-1;
	if(index1<0 || index1>nlist-1 || index2<0 || index2>nlist-1)return false;
	if(index1==index2)return true;
	T tmp=list[index1];
	list[index1]=list[index2];
	list[index2]=tmp;
	return true;
}

template <class T> void SimpleList<T>::clear()
{
	for(int i=0;i<nlist;i++){
		SimpleList_deleteValue(list[i]);
	}
	free(list);
	nlist=0;
}


template <class T> void SimpleList<T>::sort(COMPPROC comp)
{
	qsort(list,nlist,sizeof(T),comp);
}

template <class T> bool SimpleList<T>::exist(const T& value)
{
	return (indexOf(value)>=0);
}

template <class T> int SimpleList<T>::indexOf(const T& value)
{
	for(int i=0;i<nlist;i++){
		if(SimpleList_equalValue(list[i],value))
			return i;
	}
	return -1;
}
