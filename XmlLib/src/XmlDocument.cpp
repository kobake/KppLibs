#include "_required.h"
#include <shlwapi.h>
#include <tchar.h>
#include "XmlDocument.h"
#include "XmlElement.h"

#define _CLASS_MSXMLDOC_ (__uuidof(MSXML2::DOMDocument30))

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! 何もしない
XmlDocument::XmlDocument()
{
}

//! IXMLDOMDocumentPtrをラップする。
XmlDocument::XmlDocument(DOMDocPtr& pDocument)
: m_pDocument(pDocument)
{
}

//! ルートタグ名を指定し、新しいXMLDocumentを作成する。
XmlDocument::XmlDocument(const _bstr_t& rootTagName)
: m_pDocument(_CLASS_MSXMLDOC_)
{
	// 新規作成時はUTF-8以外に変更できない
	MSXML2::IXMLDOMNodePtr pNode = m_pDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding=\"UTF-8\"");
	m_pDocument->preserveWhiteSpace = VARIANT_FALSE;
	m_pDocument->appendChild(pNode);
	
	MSXML2::IXMLDOMNodePtr pRoot = m_pDocument->createElement(rootTagName);
	m_pDocument->appendChild(pRoot);
}

XmlDocument::~XmlDocument()
{
	m_pDocument=NULL;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           入力                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

XmlDocument XmlDocument::load(const _bstr_t& uri)
{
	try
	{
		DOMDocPtr pDocument(_CLASS_MSXMLDOC_);

		pDocument->validateOnParse = VARIANT_FALSE;
		pDocument->resolveExternals = VARIANT_FALSE;
		pDocument->async = VARIANT_FALSE;
		pDocument->preserveWhiteSpace = VARIANT_FALSE;
		if(!pDocument->load(uri))
		{
			throw _com_error(S_FALSE);
		}
		return XmlDocument(pDocument);
	}
	catch(_com_error&)
	{
		//TRACE2(_T("XmlDocument::load(%s)[uri] exception catched - %s"),(LPCTSTR)uri,(LPCTSTR)pCE.ErrorMessage());
		throw;
	}
}

XmlDocument XmlDocument::load(IStream* stream)
{
	try
	{
		DOMDocPtr pDocument(_CLASS_MSXMLDOC_);

		pDocument->validateOnParse = VARIANT_FALSE;
		pDocument->resolveExternals = VARIANT_FALSE;
		pDocument->async = VARIANT_FALSE;
		pDocument->preserveWhiteSpace = VARIANT_FALSE;
		if(!pDocument->load(stream))
		{
			throw _com_error(S_FALSE);
		}
		return XmlDocument(pDocument);
	}
	catch(_com_error&)
	{
		//TRACE1(_T("XmlDocument::load()[stream] exception catched - %s"),(LPCTSTR)pCE.ErrorMessage());
		throw;
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           出力                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void XmlDocument::save(const _bstr_t& filename)
{
	m_pDocument->async = VARIANT_FALSE;
	m_pDocument->save(filename);
}

void XmlDocument::save(IStream* stream)
{
	m_pDocument->async = VARIANT_FALSE;
	m_pDocument->save(stream);
}

void XmlDocument::saveEx(const _bstr_t& filename)
{
	IStreamPtr	pStream;
	HRESULT		hR;

	hR=SHCreateStreamOnFileW(filename,STGM_READWRITE|STGM_SHARE_DENY_WRITE|STGM_CREATE,&pStream);
	if(FAILED(hR)){ throw _com_error(hR); }

	MSXML2::IMXWriterPtr cW;
	cW.CreateInstance(_T("MSXML2.MXXMLWriter"));
	cW->put_version(L"1.0");
	cW->put_encoding(L"UTF-8");
	cW->put_standalone(VARIANT_TRUE);
	cW->put_indent(VARIANT_TRUE);
	cW->put_output(_variant_t((IUnknown*)(IUnknownPtr)pStream));

	MSXML2::ISAXXMLReaderPtr cR;
	cR.CreateInstance(_T("MSXML2.SAXXMLReader"));
	cR->putContentHandler((MSXML2::ISAXContentHandlerPtr)cW);
	cR->parse(_variant_t((IUnknown*)(IUnknownPtr)m_pDocument));

	cW=NULL;
	cR=NULL;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          ノード                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

XmlElement XmlDocument::root()
{
	_bstr_t name=m_pDocument->GetnodeName();
	MSXML2::IXMLDOMParseErrorPtr pErr=m_pDocument->GetparseError();
	_bstr_t errReason=pErr->Getreason();
	VARIANT_BOOL fHasChild=m_pDocument->hasChildNodes();
	MSXML2::IXMLDOMElementPtr pElem;
	pElem=m_pDocument->GetdocumentElement();
//	pElem=m_pDocument->documentElement;
	return XmlElement(pElem);
}
