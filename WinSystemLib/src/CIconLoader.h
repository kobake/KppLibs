#pragma once

class CIconLoader{
public:
	HICON LoadIconOfFile(const wchar_t* szPath);	//例：「C:\abc\def.txt」
	HICON LoadIconOfExt(const wchar_t* szExt);		//例：「.txt」
	HICON LoadIconOfFolder();
};
