#pragma once

#include "std.h"

namespace util{

template <class T> void shuffle_array(T* array,int len)
{
	int n,m;
	for(int i=0;i<len-1;i++){
		n=irand()%len;
		m=n+(irand()%(len-1)); m%=len;
		swap(array[n],array[m]);
	}
}

template <class T> void sort_array(T* array,int len) //’á‘¬¿°Ä
{
	int i,j,n,m;
	for(i=0;i<len;i++){
		for(j=i;j<len-1;j++){
			if(array[i]>array[j])swap(array[i],array[j]);
		}
	}
}

} //namespace util
