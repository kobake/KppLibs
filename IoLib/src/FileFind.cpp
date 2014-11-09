#include "_required.h"
#include "FileFind.h"

static bool is_dot(const wchar_t* ftitle)
{
	const wchar_t* p=ftitle;
	if(*p==L'.'){
		p++;
		if(*p==L'\0')return true;
		if(*p==L'.' && *(p+1)==L'\0')return true;
	}
	return false;
}

static void to_localtime(WIN32_FIND_DATA* fd)
{
	FileTimeToLocalFileTime(&fd->ftCreationTime,&fd->ftCreationTime);
	FileTimeToLocalFileTime(&fd->ftLastAccessTime,&fd->ftLastAccessTime);
	FileTimeToLocalFileTime(&fd->ftLastWriteTime,&fd->ftLastWriteTime);
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

FileFind::FileFind()
{
	hFind=NULL;
}

FileFind::~FileFind()
{
	findClose();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool FileFind::findFirst(const wchar_t* fdir,const wchar_t* ftitle)
{
	static wchar_t buf[_MAX_PATH];
	wcscpy_s(buf,countof(buf),fdir);
	wcscat_s(buf,countof(buf),L"\\");
	wcscat_s(buf,countof(buf),ftitle);
	return findFirst(buf);
}

bool FileFind::findFirst(const wchar_t* fpath)
{
	if(hFind!=NULL)findClose();

	//調べる
	hFind=FindFirstFile(fpath,&finddata);

	//無かった
	if(hFind==INVALID_HANDLE_VALUE){
		hFind=NULL;
	}
	//有った
	else{
		path.SetPath(fpath);

		if(is_dot(finddata.cFileName)){
			return findNext();
		}

		path.SetTitle(finddata.cFileName);
		to_localtime(&finddata);
	}

	return hFind!=NULL;
}

bool FileFind::findNext()
{
	bool ret;

	if(hFind==NULL)return false;

again:
	ret=(FindNextFile(hFind,&finddata)!=NULL);
	if(ret){
		if(is_dot(finddata.cFileName)){
			goto again;
		}

		path.SetTitle(finddata.cFileName);
		to_localtime(&finddata);
	}else{
		findClose();
	}

	return hFind!=NULL;
}

void FileFind::findClose()
{
	if(hFind!=NULL && hFind!=INVALID_HANDLE_VALUE)FindClose(hFind);
	hFind=NULL;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           取得                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool FileFind::isFolder() const
{
	if(hFind==NULL)return false;
	return (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0;
}
