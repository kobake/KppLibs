#include "_required.h"
#include "CRegType.h"
#include "CRegException.h"
#include <map>
#include <string>
using namespace std;

CRegType::CRegType(DWORD _type)
: type(_type)
{
}

CRegType::CRegType(const wchar_t* _type_name)
{
	//ëSéÌóﬁ: REG_NONE,REG_SZ,REG_EXPAND_SZ,REG_BINARY,REG_DWORD,REG_DWORD_LITTLE_ENDIAN,REG_DWORD_BIG_ENDIAN,REG_LINK,REG_MULTI_SZ,REG_RESOURCE_LIST,REG_FULL_RESOURCE_DESCRIPTOR,REG_RESOURCE_REQUIREMENTS_LIST
	static map<wstring,DWORD> type_table;
	if(type_table.size()==0){
		typedef pair<wstring,DWORD> type_pair;
		#define TYPE(T) type_table.insert(type_pair(L#T,T))
		TYPE(REG_SZ);
		TYPE(REG_BINARY);
		TYPE(REG_DWORD);
		TYPE(REG_EXPAND_SZ);
	}
	//ÉãÅ[ÉgñºÇÃåüçı
	map<wstring,DWORD>::iterator p=type_table.find(_type_name);
	if(p!=type_table.end()){
		type=(*p).second;
	}else{
		throw CRegException(L"CRegType: unsupported type name");
	}
}
