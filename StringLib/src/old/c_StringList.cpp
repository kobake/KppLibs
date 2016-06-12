#include "c_StringList.h"



bool StringList::put(int index,const wstring& value,int begin,int len)
{
	return put(index,value.c_str(),begin,len);
}

bool StringList::put(int index,const wchar *value,int begin,int len)
{
	//‘}“üˆÊ’u
	list<wstring>::iterator p;
	if(index==-1)p=table.end();
	else{
		p=table.begin();
		for(int i=0;i<index;i++){
			p++;
		}
	}

	//‘}“ü
	if(len==-1){
		table.insert(p,&value[begin]);
	}else{
		table.insert(p,wstring(&value[begin],len));
	}
	return true;
}

StringList *split(wchar *str,wchar *delimiter)
{
	int len_delimiter=wcslen(delimiter);
	StringList *ret=new StringList();
	wchar *p,*q;
	p=str;
	while(1){
		q=wcswcs(p,delimiter);
		if(q!=NULL){
			ret->put(-1,p,0,q-p);
			p=q+len_delimiter;
		}else{
			ret->put(-1,p);
			break;
		}
	}
	return ret;
}

StringList* StringList::clone()
{
	StringList* ret=new StringList();
	ret->table=this->table;
	return ret;
}

bool StringList::write(FileStream *out)
{
	int i,n;
	out->writeInt(n=size());
	for(i=0;i<n;i++){
		out->writeTextFormat(at(i).c_str());
	}
	return true;
}

bool StringList::read(FileStream *in)
{
	int i,n; wchar *str;
	table.clear();
	n=in->readInt();
	for(i=0;i<n;i++){
		str=in->readTextFormatAlloc();
		put(-1,str);
		free(str);
	}
	return true;
}


size_t StringList::size() const
{
	return table.size();
}

const wstring& StringList::at(size_t index) const
{
	list<wstring>::const_iterator p=table.begin();
	for(size_t i=0;i<index;i++){
		p++;
	}
	return *p;
}

bool StringList::exist(const wchar* str) const
{
	return indexOf(str)>=0;
}

#include <algorithm>

int StringList::indexOf(const wchar* str) const
{
	int i=0;
	list<wstring>::const_iterator p=table.begin(),q=table.end();
	while(p!=q){
		if(p->compare(str)==0)return i;
		p++;
		i++;
	}
	return -1;
}

void StringList::remove(int index)
{
	//íœˆÊ’u
	list<wstring>::iterator p;
	if(index==-1)p=table.end();
	else{
		p=table.begin();
		for(int i=0;i<index;i++){
			p++;
		}
	}

	//íœ
	table.erase(p);
}
