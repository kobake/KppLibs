#include "_required.h"
#include "CDropFiles.h"
using namespace std;

CDropFiles::CDropFiles(HDROP _hDrop)
: hDrop(_hDrop)
{
	num=DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
}

CDropFiles::~CDropFiles()
{
}

void CDropFiles::finish()
{
	DragFinish(hDrop);
}

size_t CDropFiles::size() const
{
	return num;
}

const wchar_t* CDropFiles::GetPath(size_t index) const
{
	static wchar_t buf[_MAX_PATH];
	if(index<num){
		DragQueryFile(hDrop,(UINT)index,buf,countof(buf));
		return buf;
	}
	else{
		return NULL;
	}
}



std::vector<std::wstring> GetDropFiles(HDROP hDrop)
{
	vector<wstring> ret;
	int nCnt = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	for(int i=0;i<nCnt;i++){
		wchar_t buf[1024]; //###’ˆÓ
		::DragQueryFile(hDrop, i, buf, _countof(buf));
		ret.push_back(buf);
	}
	::DragFinish(hDrop);
	return ret;
}

