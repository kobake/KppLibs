#include "_required.h"
#include "OutputData.h"
#include "CFileStream.h"

void OutputData::load(const wchar_t* fname)
{
	CFileOutputStream out(fname,L"wb");
	write(out);
}

