#include <BaseLib.h>
#include <LogLib.h>
#include "util/CJson.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

int wmain(int argc, wchar_t* argv[])
{
	BaseLib::Init(argc, argv);
	CLogManager::Instance()->Init();

	// json読み込みサンプル //
	CJson j;
	j.LoadJsonFile(L"あいう.json");
	printf("name:%ls\norder:%d\n\n", j.GetString(L"name").c_str(), j.GetInt(L"order"));
	printf("%ls\n", j.GetString(L"なっと").c_str());
	printf("%d\n", j.GetInt(L"たら"));
	printf("%d\n", j.GetInt(L"たら２"));
	printf("%ls\n", j.GetString(L"ほげ").c_str());
	printf("%ls\n", j.GetString(L"たら").c_str());
	printf("%ls\n", j.GetString(L"order").c_str());

	return 0;
}
