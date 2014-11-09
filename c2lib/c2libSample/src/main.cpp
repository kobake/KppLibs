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

	// jsonì«Ç›çûÇ›ÉTÉìÉvÉã //
	CJson j;
	j.LoadJsonFile(L"Ç†Ç¢Ç§.json");
	printf("name:%ls\norder:%d\n\n", j.GetString(L"name").c_str(), j.GetInt(L"order"));
	printf("%ls\n", j.GetString(L"Ç»Ç¡Ç∆").c_str());
	printf("%d\n", j.GetInt(L"ÇΩÇÁ"));
	printf("%d\n", j.GetInt(L"ÇΩÇÁÇQ"));
	printf("%ls\n", j.GetString(L"ÇŸÇ∞").c_str());
	printf("%ls\n", j.GetString(L"ÇΩÇÁ").c_str());
	printf("%ls\n", j.GetString(L"order").c_str());

	return 0;
}
