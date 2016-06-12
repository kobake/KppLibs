#pragma once

#include <BaseLib.h> // type.h
#include <IoLib.h> // c_FileStream
#include <list>
using namespace std;



class wstring_greater{
public:
	bool operator() (const wstring& lhs,const wstring& rhs) const{ return lhs.compare(rhs)>0; }
};

class wstring_lower{
public:
	bool operator() (const wstring& lhs,const wstring& rhs) const{ return lhs.compare(rhs)<0; }
};

class wstring_igreater{
public:
	bool operator() (const wstring& lhs,const wstring& rhs) const{ return lhs.compare(rhs)>0; }
};

class wstring_ilower{
public:
	bool operator() (const wstring& lhs,const wstring& rhs) const{ return lhs.compare(rhs)<0; }
};

class StringList{
public:
	bool put(int index,const wchar *value,int begin=0,int len=-1);
	bool put(int index,const wstring& value,int begin=0,int len=-1);
	static int compAZ(const wchar **s1,const wchar **s2){ return wcscmp(*s1,*s2); }
	static int compZA(const wchar **s1,const wchar **s2){ return wcscmp(*s2,*s1); }
	static int compiAZ(const wchar **s1,const wchar **s2){ return wcsicmp(*s1,*s2); }
	static int compiZA(const wchar **s1,const wchar **s2){ return wcsicmp(*s2,*s1); }
	void sortAZ(){ table.sort(wstring_greater()); }
	void sortZA(){ table.sort(wstring_lower()); }
	void sortiAZ(){ table.sort(wstring_igreater()); }
	void sortiZA(){ table.sort(wstring_ilower()); }
	StringList* clone();
	//
	size_t size() const;
	const wstring& at(size_t index) const;
	//
	bool exist(const wchar* str) const;
	int indexOf(const wchar* str) const;
	void remove(int index);
	//
	bool write(FileStream *out);
	bool read(FileStream *in);
private:
	list<wstring> table;
};



StringList *split(wchar *str,wchar *delimiter);
