#include "_required.h"
#include <shlwapi.h>
#include <tchar.h>
#include "XmlElementList.h"
#include "XmlElement.h"

void XmlElementList::add(MSXML2::IXMLDOMElementPtr &pElement)
{
	_vTags.push_back(pElement);
}

XmlElement XmlElementList::child(const _bstr_t& bszTagName) throw(_com_error)
{
	int	nIdx=0;
	int	nSize = (int)_vTags.size();

	for(nIdx=0;nIdx<nSize;nIdx++)
	{
		try
		{
			return _vTags[nIdx].child(bszTagName);
		}
		catch(_com_error &)
		{
			//TRACE1(_T("XmlElementList::child(%s) exception catched"),(LPCTSTR)bszTagName);
			throw _com_error(E_FAIL);
		}
	}
	
	// 見つからなかったときは例外を発生させる
	throw _com_error(E_FAIL);
}

XmlElementList XmlElementList::childList(const _bstr_t& bszTagName)
{
	XmlElementList ctlRet;
	int	nSize=(int)_vTags.size();
	int	nIdx=0;

	for(nIdx=0;nIdx<nSize;nIdx++)
	{
		XmlElementList ctlChildList = _vTags[nIdx].childList(bszTagName);
		ctlRet._vTags.insert(ctlRet._vTags.end(),ctlChildList._vTags.begin(),ctlChildList._vTags.end());
	}
	// 見つからなかったときはサイズ0の配列が返る
	return ctlRet;
}
