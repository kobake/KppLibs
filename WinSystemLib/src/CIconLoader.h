#pragma once

class CIconLoader{
public:
	HICON LoadIconOfFile(const wchar_t* szPath);	//��F�uC:\abc\def.txt�v
	HICON LoadIconOfExt(const wchar_t* szExt);		//��F�u.txt�v
	HICON LoadIconOfFolder();
};
