#pragma once

#include "util/vector_ex.h"

//‹L˜^‚à‚·‚é
template <class T> class TInstanceHolder{
public:
	TInstanceHolder()
	{
		gm_table.push_back(static_cast<T*>(this));
	}
	virtual ~TInstanceHolder()
	{
		gm_table.erase_value(static_cast<T*>(this));
	}
	static int InstanceCount(){ return (int)gm_table.size(); }
	static T* Instance(int nIndex)
	{
		if(nIndex>=0 && nIndex<(int)gm_table.size()){
			return gm_table[nIndex];
		}else{
			return NULL;
		}
	}

private:
	static vector_ex<T*> gm_table;
};

template <class T> vector_ex<T*> TInstanceHolder<T>::gm_table;
