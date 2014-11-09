#include <c2lib.h>
#include <WinSystemLib.h>
#include <locale.h>

int wmain(int argc, wchar_t* argv[])
{
	wchar_t buf[_MAX_PATH] = L"";
	setlocale(LC_ALL, "");
	c2lib::Init(argc, argv);
	// レジストリ //
	if(0){
		//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\WallPaperDir
		CRegKey cKey(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion");
		ExpandEnvironmentStrings(cKey.GetValue(L"WallPaperDir").ToString().c_str(), buf, _countof(buf));
		printf("%ls\n", buf);
	}
	// シェル //
	if(1){
//		CShellFolder s(L"D:\\_tmp");
		CShellFolder s(L"::MyComputer");
		CEnumIdList list = s.CreateEnumIDList();
		while(1){
			CItemIdList item = list.CreateNextIDList();
			if(!item.IsValid())break;
			std::wstring strName = s.GetDisplayNameOfItem(item);//item.GetPath();
			std::wstring strPath = item.GetPath();
			wprintf(L"[%ls] [%ls]\n", strName.c_str(), strPath.c_str());
		}
	}
	return 0;
}
//ProgramFilesPath
