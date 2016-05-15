#pragma once

#define m_swap(n1,n2,tmp) do{ tmp=n1; n1=n2; n2=tmp; }while(0)

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

/*
template <class T> T name_to_value(const wchar *name,T def_value,const wchar *name_table[],const T value_table[])
{
	int i;
	for(i=0;name_table[i]!=NULL;i++){
		if(strcmp(name,name_table[i])==0){
			return value_table[i];
		}
	}
	return def_value;
}

template <class T> const wchar* value_to_name(T value,const wchar *def_name,const wchar *name_table[],const T value_table[])
{
	int i;
	for(i=0;name_table[i]!=NULL;i++){
		if(value==value_table[i]){
			return name_table[i];
		}
	}
	return def_name;
}
*/
template <class T> struct NAME_VALUE_PAIR
{
	wchar *name;
	T value;
};

template <class T> T name_to_value(const wchar *name,T def_value,NAME_VALUE_PAIR<T> name_value_table[])
{
	int i;
	for(i=0;name_value_table[i].name!=NULL;i++){
		if(wcscmp(name,name_value_table[i].name)==0){
			return name_value_table[i].value;
		}
	}
	return def_value;
}
template <class T> const wchar* value_to_name(T value,const wchar *def_name,NAME_VALUE_PAIR<T> name_value_table[])
{
	int i;
	for(i=0;name_value_table[i].name!=NULL;i++){
		if(value==name_value_table[i].value){
			return name_value_table[i].name;
		}
	}
	return def_name;
}


