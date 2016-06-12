#pragma once

#include <BaseLib.h> //c_SystemTime

class FileDetail{
protected:
	WIN32_FIND_DATA *finddata;
public:
	FileDetail(WIN32_FIND_DATA *_finddata);
	SystemTime *getCreateTime();
	SystemTime *getAccessTime();
	SystemTime *getWriteTime();
	int GetSize();
	bool isFolder();
	void toFolder();
	bool isArchive();
	bool isHidden();
	bool isNormal();
	bool isSystem();
	wchar *getTitle();
	wchar *getExt();
};
