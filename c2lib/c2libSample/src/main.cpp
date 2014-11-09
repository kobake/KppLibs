#include <c2lib.h>
#include <LogLib.h>
#include "util/CJson.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	CLogManager::Instance()->Init();

	// json�ǂݍ��݃T���v�� //
	CJson j;
	j.LoadJsonFile(L"������.json");
	printf("name:%ls\norder:%d\n\n", j.GetString(L"name").c_str(), j.GetInt(L"order"));
	printf("%ls\n", j.GetString(L"�Ȃ���").c_str());
	printf("%d\n", j.GetInt(L"����"));
	printf("%d\n", j.GetInt(L"����Q"));
	printf("%ls\n", j.GetString(L"�ق�").c_str());
	printf("%ls\n", j.GetString(L"����").c_str());
	printf("%ls\n", j.GetString(L"order").c_str());

	return 0;
}
