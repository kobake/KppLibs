#include "_required.h"
#include "CRegHive.h"
#include "CRegException.h"
#include <map>
using namespace std;

static map<wstring,HKEY> g_n2h;
static map<HKEY,wstring> g_h2n;

static void _init()
{
	if(g_n2h.size()==0){
		typedef pair<wstring,HKEY> n2h_pair;
		typedef pair<HKEY,wstring> h2n_pair;
		#define ROOT(R) g_n2h.insert(n2h_pair(LTEXT(#R),R)); g_h2n.insert(h2n_pair(R,LTEXT(#R)))
		ROOT(HKEY_CLASSES_ROOT);
		ROOT(HKEY_CURRENT_USER);
		ROOT(HKEY_LOCAL_MACHINE);
		ROOT(HKEY_USERS);
		ROOT(HKEY_PERFORMANCE_DATA);
		ROOT(HKEY_CURRENT_CONFIG);
		ROOT(HKEY_DYN_DATA);
		//ROOT(HKEY_PERF_ROOT);
	}
}

static HKEY _NameToHandle(const wchar_t* szName)
{
	_init();
	map<wstring,HKEY>::const_iterator p = g_n2h.find(szName);
	if(p!=g_n2h.end()){
		return p->second;
	}
	else{
		return NULL;
	}
}

static const wchar_t* _HandleToName(HKEY hKey)
{
	_init();
	map<HKEY,wstring>::const_iterator p = g_h2n.find(hKey);
	if(p!=g_h2n.end()){
		return p->second.c_str();
	}
	else{
		return L"";
	}
}

CRegHive::CRegHive(const wchar_t* _root_name)
{
	//root_nameÇÃ"\\"à»ç~ÇéÊÇËèúÇ≠
	const wchar_t* t=wcschr(_root_name,L'\\');
	wstring root_name;
	if(t){
		root_name.assign(_root_name,t-_root_name);
	}
	else{
		root_name.assign(_root_name);
	}

	//ÉãÅ[ÉgñºÇÃåüçı
	m_hKey = _NameToHandle(root_name.c_str());
	if(m_hKey==NULL){
		throw CRegException(L"CRegHive: invalid hive name [%s]",root_name.c_str());
	}
}

CRegHive::CRegHive(HKEY _hRootKey)
{
	m_hKey=_hRootKey;
}

CRegHive::CRegHive()
{
	m_hKey=NULL;
}

const wchar_t* CRegHive::ToString() const
{
	return _HandleToName(m_hKey);
}
