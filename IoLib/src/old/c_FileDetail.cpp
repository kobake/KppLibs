#include <BaseLib.h>
#include <windows.h>
#include "c_FileDetail.h"

static SystemTime *TempSystemTime()
{
	static SystemTime tmp1,tmp2,*ret=&tmp2;
	if(ret==&tmp1)return ret=&tmp2;
	else return ret=&tmp1;
}

FileDetail::FileDetail(WIN32_FIND_DATA *_finddata)
{
	finddata=_finddata;
}

SystemTime *FileDetail::getCreateTime()
{
	SystemTime *systemtime=TempSystemTime();
	systemtime->setFileTime(&finddata->ftCreationTime);
	return systemtime;
}

SystemTime *FileDetail::getAccessTime()
{
	SystemTime *systemtime=TempSystemTime();
	systemtime->setFileTime(&finddata->ftLastAccessTime);
	return systemtime;
}

SystemTime *FileDetail::getWriteTime()
{
	SystemTime *systemtime=TempSystemTime();
	systemtime->setFileTime(&finddata->ftLastWriteTime);
	return systemtime;
}

int FileDetail::GetSize()
{
	return finddata->nFileSizeLow;
}

bool FileDetail::isFolder()
{
	return 0!=(finddata->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}
void FileDetail::toFolder()
{
	finddata->dwFileAttributes|=FILE_ATTRIBUTE_DIRECTORY;
}

bool FileDetail::isArchive()
{
	return 0!=(finddata->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
}

bool FileDetail::isHidden()
{
	return 0!=(finddata->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
}

bool FileDetail::isNormal()
{
	return 0!=(finddata->dwFileAttributes & FILE_ATTRIBUTE_NORMAL);
}

bool FileDetail::isSystem()
{
	return 0!=(finddata->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
}

wchar *FileDetail::getTitle()
{
	return finddata->cFileName;
}

wchar *FileDetail::getExt()
{
	wchar *p=wcsrchr(finddata->cFileName,L'.');
	if(p!=NULL)
		return p+1;
	else
		return L"";
}
