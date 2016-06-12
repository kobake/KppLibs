#pragma once

#include <string>
#include "../std/myexception.h"
using namespace std;

//マクロ版 min,max
#define mmin(n1,n2) ( ((n1)<(n2))?(n1):(n2) )
#define mmax(n1,n2) ( ((n1)>(n2))?(n1):(n2) )

namespace util{

	//テンプレート版 min, max
	template <class T> const T& tmin(const T& t1, const T& t2)
	{
		if(t1<t2)return t1;
		else return t2;
	}

	template <class T> const T& tmax(const T& t1, const T& t2)
	{
		if(t1>t2)return t1;
		else return t2;
	}

	template <class T> T tabs(T t)
	{
		if(t<0)return -t; return t;
	}

	template <class T> T unit(T t)
	{
		if(t<0)return -1;
		else if(t>0)return 1;
		else return 0;
	}

	template <class T> void swap(T& t1, T& t2)
	{
		T t3=t1;
		t1=t2;
		t2=t3;
	}

	inline int tabs(int n)
	{
		if(n<0)return -n;
		return n;
	}

	int irand();
	int irand(int from, int to);
	double drand(double dmin, double dmax);
	double drand();

} //namespace util

using namespace util;
