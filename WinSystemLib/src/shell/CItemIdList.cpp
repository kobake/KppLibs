#include <BaseLib.h>
#include "CItemIdList.h"
#include "CMalloc.h"
#include "CShellFolder.h"
#include <shlobj.h>
#include <DebugLib.h>
#include <LogLib.h>

//関数ユーティリティ
class CItemIdListRef{
public:
	CItemIdListRef(LPCITEMIDLIST pidl)
	: m_pidl(pidl)
	{
	}


	UINT GetSizeIncludeNull()
	{
		if(!m_pidl)return 0;
		
		UINT nSize = 0;

		while(m_pidl->mkid.cb){
			nSize+=m_pidl->mkid.cb;
			m_pidl=Next(m_pidl);
		}

		nSize+=sizeof(m_pidl->mkid.cb); //終端ヌル分
		return nSize;
	}

	static LPCITEMIDLIST Next(LPCITEMIDLIST pidl)
	{
		return (LPCITEMIDLIST)((BYTE*)pidl+pidl->mkid.cb);
	}

	int GetCount() const
	{
		if(!m_pidl)return 0;

		int nCount = 0;
		LPCITEMIDLIST pCur = m_pidl;
		while(pCur->mkid.cb)
		{
			nCount++;
			pCur = (LPCITEMIDLIST)((BYTE*)pCur + pCur->mkid.cb);
		}
		return nCount;
	}
	int GetSize(int nCount=-1) const
	{
		if(!m_pidl)return 0;
		if(nCount==-1)nCount=GetCount();
		
		int nSize = 0;
		int i = 0;
		LPCITEMIDLIST pidl = m_pidl;

		while(pidl->mkid.cb && i<nCount){
			nSize+=pidl->mkid.cb;
			i++;
			pidl=Next(pidl);
		}

		return nSize;
	}
	LPITEMIDLIST Clone(int nCount = -1)
	{
		if(!m_pidl)return NULL;
		if(nCount==-1)nCount=GetCount();

		int nSize = GetSize(nCount);

		CMalloc cMalloc;
		LPITEMIDLIST pidlNew=(LPITEMIDLIST)cMalloc.Alloc(nSize+sizeof(USHORT));	//確保
		memcpy(pidlNew,m_pidl,nSize);											//コピー
		((LPITEMIDLIST)((BYTE*)pidlNew+nSize))->mkid.cb=0;						//終端ヌル
		
		return pidlNew;
	}
private:
	LPCITEMIDLIST m_pidl;
};

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CItemIdList::CItemIdList()
{
	m_pidl = NULL;
}

CItemIdList::CItemIdList(const wchar_t* szPath)
{
	m_pidl = NULL;
	SFGAOF sfgao = {0};
	SHParseDisplayName(szPath, NULL, &m_pidl, 0, &sfgao);	// フルパスなITEMIDLISTを構築 //

	//###
	m_debug = this->GetPath(); // フルパスが構築される //
}

CItemIdList::CItemIdList(LPCITEMIDLIST pidl, int nCount)
{
	m_pidl = NULL;
	Assign(pidl,nCount);
}

CItemIdList::CItemIdList(const CItemIdList& rhs)
{
	m_pidl = NULL;
	Assign(rhs.m_pidl);
}

CItemIdList& CItemIdList::operator = (const CItemIdList& rhs)
{
	Assign(rhs.m_pidl);
	return *this;
}

CItemIdList::~CItemIdList()
{
	_Dispose();
}

void CItemIdList::_Dispose()
{
	if(m_pidl){
		CMalloc().Free(m_pidl);
		m_pidl = NULL;
	}
}

void CItemIdList::Assign(LPCITEMIDLIST pidl, int nCount)
{
	_Dispose();

	m_pidl = CItemIdListRef(pidl).Clone(nCount);

	//###
	m_debug = this->GetPath();
}

void CItemIdList::Attach(LPITEMIDLIST pidl)
{
	_Dispose();
	m_pidl = pidl;

	//###
	m_debug = this->GetPath();
}

//! 所有権を放棄する
LPITEMIDLIST CItemIdList::Detach()
{
	LPITEMIDLIST ret = m_pidl;
	m_pidl = NULL;

	//###
	m_debug = L"";

	return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           操作                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CItemIdList::Clear()
{
	this->_Dispose();
	ITEMIDLIST tmp;
	tmp.mkid.cb = 0;
	this->Assign(&tmp,0);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

std::wstring CItemIdList::GetPath() const
{
	wchar_t buf[_MAX_PATH] = L"";
	::SHGetPathFromIDList(m_pidl,buf);
	return buf;
}

std::wstring CItemIdList::GetPath(const CShellFolder& cFolder) const
{
//	wchar_t buf[_MAX_PATH] = L"";
	return cFolder.GetDisplayNameOfItem(*this);
//	cFolder._GetIShellFolder()->
//	::SHGetPathFromIDList(m_pidl,buf);
//	return buf;
}

int CItemIdList::GetCount() const
{
	return CItemIdListRef(m_pidl).GetCount();
}

LPCITEMIDLIST CItemIdList::GetAt(int nPos) const
{
	if (!m_pidl)
		return 0;

	int nCount = 0;
	
	LPCITEMIDLIST pCur = m_pidl;
	while(pCur->mkid.cb)
	{
		if(nCount==nPos)
			return pCur;

		nCount++;
		pCur = (LPCITEMIDLIST)((BYTE*)pCur + pCur->mkid.cb);
	}
	if(nCount==nPos)
		return pCur;
	return NULL;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    関数ユーティリティ                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include <vector>
#include <string>
using namespace std;

static void _Dump(const void* pData, int nLen)
{
	string tmp;
	wstring tmp2;
	vector<char> vData(nLen+2);
	for(int i=0;i<nLen;i++)vData[i] = ((const char*)pData)[i];
	vData[nLen] = 0;
	vData[nLen+1] = 0;
	//char表現
	tmp="";
	{
		const char* p = (const char*)pData;
		for(int i=0;i<nLen;i++){
			char buf[32];
			sprintf(buf, "%c", *p);
			while(strlen(buf)<2)strcat(buf," ");
			p++;
			tmp+=buf;
		}
	}
	debugPrint("[Chr] ");
	debugPrint(tmp.c_str());
	debugPrint("\r\n");
	//wchar_t表現
	tmp2=L"";
	{
		const wchar_t* p = (const wchar_t*)pData;
		for(int i=0;i<nLen/2;i++){
			wchar_t buf[32];
			swprintf(buf, L"%lc", *p);
			while(wcslen(buf)<4)wcscat(buf,L" ");
			p++;
			tmp2+=buf;
		}
	}
	debugPrint(L"[Wid] ");
	debugPrint(tmp2.c_str());
	debugPrint(L"\r\n");
	//16進表現
	tmp = "";
	{
		const BYTE* p = (const BYTE*)pData;
		for(int i=0;i<nLen;i++){
			char buf[32];
			sprintf(buf, "%02X", *p);
			p++;
			tmp+=buf;
		}
	}
	KLOG(_INFO,"[Hex] %s", tmp.c_str());
	
	//終端
	KLOG(_INFO, "");
}

static void _Dump(const SHITEMID* pitem)
{
	const SHITEMID* p = pitem;
	while(p->cb){
		_Dump(p->abID, p->cb - sizeof(p->cb));
		p = (const SHITEMID*)((const char*)p + p->cb);
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    デバッグ                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#include <string>
#include <StringLib.h>
using namespace std;

void CItemIdList::_Dump() const
{
	CShellFolder cCurFolder = CDesktopFolder();
//	cCurFolder = CShellFolder(L""); //####実験

	int n = this->GetCount();
	for(int i=0;i<n;i++){
		CItemId cCopyID = this->GetAt(i)->mkid;

		// フォルダ名の取得。IShellFolder, ITEMIDLIST -> string
		wstring str = cCurFolder.GetDisplayNameOfItem(CItemIdList(cCopyID._GetItemIdList()));
		debugPrint(str.c_str());
		debugPrint(L"◆");

		// 現在のフォルダを入れ換えます。
		IShellFolder* pNewFolder = cCurFolder.CreateShellFolder(CItemIdList(cCopyID._GetItemIdList()));
		cCurFolder.Attach(pNewFolder);
		if(!cCurFolder.IsValid())
			break;
	}

	debugPrint(L"\r\n");
}
