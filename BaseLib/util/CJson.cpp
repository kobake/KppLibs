#include "CJson.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <StringLib.h>
#include <LogLib.h>

using boost::property_tree::ptree;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �������ƏI�� //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CJson::CJson()
{
	m_pTree = new ptree;
}

CJson::~CJson()
{
	delete (ptree*)m_pTree;
}

bool CJson::LoadJsonFile(const wchar_t* szFilePath)
{
	ptree& pt = *((ptree*)m_pTree);

	// SJIS�ɕϊ� //
	char szFilePathA[_MAX_PATH];
	_snprintf(szFilePathA, _countof(szFilePathA), "%ls", szFilePath);

	// ���[�h //
	try{
		read_json(szFilePathA, pt);
	}
	catch(const std::exception& e){
		KLOG(_ERROR, "read_json failed. Reason:%hs", e.what());
		return false;
	}
	catch(...){
		KLOG(_ERROR, "read_json failed. Reason:?");
		return false;
	}

	return true;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �擾 //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
/*
using namespace std ;
using namespace boost ;
using namespace boost::property_tree ;
*/

std::wstring CJson::GetString(const wchar_t* szKey) const
{
	const ptree& pt = *((ptree*)m_pTree);
	if(pt.empty())return L"";

	// �l�擾 //
	std::string str;
	try{
		ptree::const_assoc_iterator itr = pt.find(cpp_wcstombs(szKey, CP_UTF8));
		if(itr != pt.not_found()){
//			str = pt.get<std::string>(cpp_wcstombs(szKey, CP_UTF8));
			str = itr->second.get_value<std::string>();
		}
		else{
			return L"";
		}
	}
	catch(const std::exception& e){
		std::string s = e.what();
//		KLOG(_ERROR, "get failed. Reason:%hs", e.what());
		return false;
	}
	catch(...){
		KLOG(_ERROR, "get failed. Reason:?");
		return false;
	}

	// UTF-8 �� UTF-16 �ϊ� //
	return cpp_mbstowcs(str, CP_UTF8);
}

int CJson::GetInt(const wchar_t* szKey) const
{
	return _wtoi(GetString(szKey).c_str());
	/*
	const ptree& pt = *((ptree*)m_pTree);
	if(pt.empty())return 0;

	// �l�擾 //
	int i = 0;
	try{
		ptree::const_assoc_iterator itr = pt.find(cpp_wcstombs(szKey, CP_UTF8));
		if(itr != pt.not_found()){
//			i = itr->second.get_value<int>();
			const ptree& test = itr->second;
			boost::optional<int> opt = pt.get_value_optional<int>();
			if(opt){
				i = opt.get();
			}
			else{
				return 0;
			}
//			i = pt.get<int>(cpp_wcstombs(szKey), CP_UTF8);
		}
		else{
			return 0;
		}
	}
	catch(const std::exception& e){
		KLOG(_ERROR, "get failed. Reason:%hs", e.what());
		return false;
	}
	catch(...){
		KLOG(_ERROR, "get failed. Reason:?");
		return false;
	}

	// ���� //
	return i;
	*/
}

