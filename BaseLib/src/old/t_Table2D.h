#pragma once

#include "t_base.h"

template <class T> class Table2D{
protected:
	T *table;
	int w;
	int h;
	T err_value;
public:
	Table2D(int _w,int _h)
	{
		table=NULL;
		w=0;
		h=0;
		memset(&err_value,0,sizeof(T));
		setSize(_w,_h,true);
	}
	virtual ~Table2D()
	{
		m_free(table);
	}
	void setErrorValue(T _err_value)
	{
		err_value=_err_value;
	}
	void setSize(int _w,int _h,bool _clear)
	{
		_w=max(1,_w);
		_h=max(1,_h);
		//
		int i,j;
		int ww=tmin(w,_w);
		int hh=tmin(h,_h);
		T *new_table;
		if(_w*_h>0){
			new_table=(T*)calloc(_w*_h,sizeof(T));
		}else{
			new_table=NULL;
		}
		if(!_clear){
			for(j=0;j<hh;j++){
				for(i=0;i<ww;i++){
					new_table[j*_w+i]=table[j*w+i];
				}
			}
		}
		free(table);
		table=new_table;
		w=_w;
		h=_h;
	}
	void set(int x,int y,T value)
	{
		if(x>=0 && x<w && y>=0 && y<h)
			table[x+y*w]=value;
	}
	T get(int x,int y,T _err_value)
	{
		if(x>=0 && x<w && y>=0 && y<h)
			return table[x+y*w];
		else
			return _err_value;
	}
	T get(int x,int y)
	{
		if(x>=0 && x<w && y>=0 && y<h)
			return table[x+y*w];
		else
			return err_value;
	}
	int getWidth()
	{
		return w;
	}
	int getHeight()
	{
		return h;
	}
	int GetSize()
	{
		return w*h;
	}
};

typedef Table2D<int> IntTable2D;



