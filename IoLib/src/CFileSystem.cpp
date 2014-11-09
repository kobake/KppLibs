#include "common/_required.h"
#include "CFileSystem.h"
#include <direct.h> //_chdir
#include <string>
using namespace std;

bool CFileSystem::chdir(const wchar_t* szPath)
{
	if(szPath[0] && szPath[1]==L':' && szPath[2]==L'\0'){
		wstring strPath = (wstring)szPath + L"\\";
		return 0==_wchdir(strPath.c_str());
	}
	else{
		return 0==_wchdir(szPath);
	}
}

