#include "_required.h"
#include "InputData.h"
#include "CFileStream.h"

void InputData::load(const wchar_t* fname)
{
	CFileInputStream in(fname,L"rb");
	Read(in);
}

