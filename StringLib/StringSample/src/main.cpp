#include <c2lib.h>
#include <StringLib.h>
#include <DebugLib.h>
#include <mmsystem.h>

void extract_test(const wchar_t* str)
{
	std::wstring ext = fextract2(str, L'/');
	printf("-----------------------\n");
	printf("%ls\n", str);
	printf("↓\n");
	printf("%ls\n", ext.c_str());
}

int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	debugInit(DEBUG_OUT_DEBUGGER);

	// timeGetTimeの分解能 //
	timeBeginPeriod(1);

	int cnt = 100 * 10000;

	// 軽い文字列クラスのパフォーマンス測定 //
	{
		DWORD dwStart = timeGetTime();
		{
			std::wstring str2 = L"aaa";
			for(int i = 0; i < cnt; i++){
				std::wstring str;
				str = L"hoge";
//				str = str2;
			}
		}
		DWORD dwEnd = timeGetTime();
		DWORD dwTotal = dwEnd - dwStart;
		printf("std::wstring %d\n", dwTotal);
	}
	{
		DWORD dwStart = timeGetTime();
		{
			CLightString str2 = L"aaa";
			for(int i = 0; i < cnt; i++){
				CLightString str;
				str = L"hoge";
//				str = str2;
			}
		}
		DWORD dwEnd = timeGetTime();
		DWORD dwTotal = dwEnd - dwStart;
		printf("CLightString %d\n", dwTotal);
	}
}

int wmain_regex(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	debugInit(DEBUG_OUT_DEBUGGER);

	printf("%d\n", CRegex::Exists(L"hoge", L"o[a-z]"));
	printf("%d\n", CRegex::Exists(L"hoge", L"o[A-Z]"));

	return 0;

	extract_test(L"/abc/def/..");
	extract_test(L"/abc/def/../..");
	extract_test(L"/abc/def/../../..");

	extract_test(L"c/abc/def/..");
	extract_test(L"c/abc/def/../..");
	extract_test(L"c/abc/def/../../..");

	extract_test(L"abc/def/../ghi");
	extract_test(L"http://abc/def/../ghi/..");
	extract_test(L"http://abc/def/../ghi/../..");
	extract_test(L"http://abc/def/../ghi/../../..");
	extract_test(L"abc/def/./ghi");
	extract_test(L"abc/def/./ghi/.");
	extract_test(L"./abc/def/./ghi/.");

	

	return 0;
}

