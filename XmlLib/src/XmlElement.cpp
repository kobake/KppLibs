#include "_required.h"
#include <shlwapi.h>
#include <tchar.h>
#include "XmlElement.h"
#include "XmlElementList.h"
#include "XmlDocument.h"
#include <StringLib.h>

typedef _bstr_t bstring;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! 何もしない。
XmlElement::XmlElement()
{
}

//! IXMLDOMElementPtr をラップする。
XmlElement::XmlElement(DOMElementPtr& pElement)
: m_pElement(pElement)
{
}

XmlElement::~XmlElement()
{
	m_pElement=NULL;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        属性の取得                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! エレメント名を取得
bstring XmlElement::getTagName() const
{
	return m_pElement->nodeName;
}

//! 属性を取得
bstring	XmlElement::getAttribute(const bstring& bszName)
{
	_variant_t	vRet;

	try{
		vRet=m_pElement->getAttribute(bszName);
	}
	catch(_com_error){
//		if(e.Error()==0){}
//		throw AttributeNotFound();
		//TRACE2(_T("XmlElement::getAttribute(%s) exception catched - %s"),(LPCTSTR)bszName,(LPCTSTR)pCE.ErrorMessage());
		throw;
	}
	return (vRet.bstrVal)?vRet.bstrVal:bstring(_T(""));
}

//!< 属性リストを取得
std::map<bstring, bstring> XmlElement::getAttributes()
{
	std::map<bstring, bstring> mAttributes;

	try
	{
		MSXML2::IXMLDOMNamedNodeMapPtr pNodes = m_pElement->attributes;
	
		if(pNodes)
		{
			int nLen = pNodes->length;
			int nIdx;
			for(nIdx=0;nIdx<nLen;nIdx++)
			{
				MSXML2::IXMLDOMNodePtr pNode = pNodes->item[nIdx];
				
				_variant_t vValue(pNode->nodeValue);
				if(vValue.bstrVal)
				{
					bstring bszName((LPCWSTR)pNode->nodeName);
					mAttributes[bszName]=vValue.bstrVal;
				}
			}
		}
	
		return mAttributes;
	}
	catch(_com_error&)
	{
		//TRACE1(_T("XmlElement::getAttributes 1 exception catched - %s"),(LPCTSTR)pCE.ErrorMessage());
		return mAttributes;
	}
	catch(...)
	{
		//TRACE0(_T("XmlElement::getAttributes 2 exception catched"));
		return mAttributes;
	}
}


bstring XmlElement::getText()
{
	MSXML2::IXMLDOMNodeListPtr	pNodes = m_pElement->childNodes;
	int	nLen = pNodes->length;
	int	nIdx;

	for(nIdx=0;nIdx<nLen;nIdx++){
		MSXML2::IXMLDOMTextPtr pNode = pNodes->item[nIdx];
		
		if(pNode)
		{
			return pNode->text;
		}
	}
	
	return bstring(_T(""));
}

//! テキストの配列を取得
/*!
例：<tag1>text1<tag2>text2</tag2>test3</tag1> のときは 
tag1.getTextArray()は text1, text3 になる
*/
std::vector<bstring> XmlElement::getTextArray()
{
	MSXML2::IXMLDOMNodeListPtr pNodes = m_pElement->childNodes;
	std::vector<bstring>	vRet;
	int			nLen=pNodes->length;
	int			nIdx;

	for(nIdx=0;nIdx<nLen;nIdx++)
	{
		MSXML2::IXMLDOMTextPtr pNode=pNodes->item[nIdx];
		
		if(pNode)
		{
			vRet.push_back(pNode->text);
		}
	}
	
	return vRet;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        属性の設定                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void XmlElement::setAttribute(const bstring& bszName, const bstring& bszValue)
{
	m_pElement->setAttribute(bszName, bszValue);
}


void XmlElement::setText(const bstring& bszText)
{
	MSXML2::IXMLDOMNodeListPtr	pNodes = m_pElement->childNodes;
	int	nLen=pNodes->length;
	int	nIdx=0;

	for(nIdx=0;nIdx<nLen;nIdx++)
	{
		MSXML2::IXMLDOMTextPtr pNode=pNodes->item[nIdx];
		
		if(pNode)
		{
			pNode->text=bszText;
			return;
		}
	}
	
	MSXML2::IXMLDOMDocumentPtr pDocument=m_pElement->ownerDocument;
	MSXML2::IXMLDOMTextPtr	pChild=pDocument->createTextNode(bszText);
	m_pElement->appendChild(pChild);
}

//! テキストの配列を設定
/*!
例：<tag1><tag2></tag2><tag2></tag2></tag1> のときは
tag1.setText(text1, text2)は <tag1>text1<tag2></tag2>text2</tag1> になる
配列中に空文字があるときは、設定せずにとばす
*/
void XmlElement::setTextArray(std::vector<bstring> &textArray)
{
	MSXML2::IXMLDOMNodeListPtr pNodes = m_pElement->childNodes;
	int	nLen=pNodes->length;
	int	nIdx=0;	// ++forest: scope

	for(nIdx=0;nIdx<nLen;nIdx++)
	{
		MSXML2::IXMLDOMTextPtr pNode = pNodes->item[nIdx];

		if(pNode)
		{
			m_pElement->removeChild(pNode);
		}
	}
	
	MSXML2::IXMLDOMDocumentPtr pDocument=m_pElement->ownerDocument;
	pNodes=m_pElement->childNodes;
	std::vector<bstring>::iterator ivText=textArray.begin();
	if(ivText==textArray.end())
	{
		return;
	}
	
	nLen=pNodes->length;
	for(nIdx=0;nIdx<nLen;nIdx++)
	{
		DOMElementPtr pElement=pNodes->item[nIdx];
		
		if(pElement)
		{
			if(ivText->length())
			{
				MSXML2::IXMLDOMTextPtr pChild = pDocument->createTextNode(*ivText);
				m_pElement->insertBefore(pChild,_variant_t(pElement, true));
			}
			if(++ivText==textArray.end())
			{
				return;
			}
		}
	}
	
	do
	{
		if(ivText->length())
		{
			MSXML2::IXMLDOMTextPtr pChild=pDocument->createTextNode(*ivText);
			m_pElement->appendChild(pChild);
		}
	}
	while(++ivText!=textArray.end());
}

//! 属性を削除
void XmlElement::removeAttribute(const bstring& bszName)
{
	m_pElement->removeAttribute(bszName);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ノード取得                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

XmlElement XmlElement::getParent() throw(_com_error)
{
	// 親はElementNodeかDocumentNode（ルートタグの場合）のどちらか
	// NULLのときはない
	DOMElementPtr	pParent=m_pElement->parentNode;
	// QueryInterfaceが成功したか調べる
	if(!pParent)
	{
		throw _com_error(E_FAIL);
	}
	return XmlElement(pParent);
}


//! 子供のタグを取得
/*!
子供のタグのうちtagNameに指定されたタグ名のもの(tagNameを取得しないときは
すべての子供のタグ)で最初のものを取得する
該当するタグがないときは_com_errorが投げられる。
*/
XmlElement XmlElement::child(const bstring& tagName) throw(_com_error)
{
	MSXML2::IXMLDOMNodeListPtr nodes = m_pElement->childNodes;
	
	bool any=(tagName.length()==0); //名前が指定されていない場合は、最初に見つかったエレメントを返す。

	int len = nodes->length;
	for(int i=0;i<len;i++){
		DOMElementPtr element=nodes->item[i];
		if(element){
			if(any || bstring(element->nodeName)==tagName){
				return XmlElement(element);
			}
		}
	}
	throw _com_error(E_FAIL);
}

//! 子供のタグを取得
/*!
子供のタグのうちtagNameに指定されたタグ名のもの(tagNameを取得しないときは
すべての子供のタグ)を取得する
*/
XmlElementList XmlElement::childList(const bstring& tagName)
{
	MSXML2::IXMLDOMNodeListPtr nodes = m_pElement->childNodes;
	
	XmlElementList ret;
	int len = nodes->length;
	if(tagName.length()){
		for(int i = 0; i < len; i++){
			DOMElementPtr element = nodes->item[i];
			
			if(element){
				if(bstring(element->nodeName) == tagName){
					ret.add(element);
				}
			}
		}
	}
	else{
		for(int i = 0; i < len; i++){
			DOMElementPtr element = nodes->item[i];
			
			if(element){
				ret.add(element);
			}
		}
	}
	
	return ret;
}



//! 1つの属性条件で子エレメントを探す。
XmlElement XmlElement::findChild(const wchar_t* elt_name,const wchar_t* att_name,const wchar_t* att_value)
{
	MSXML2::IXMLDOMNodeListPtr nodes = m_pElement->childNodes;
	
	bool elt_any=wstr_empty(elt_name); //名前が指定されていない場合は、最初に見つかったエレメントを返す。
	bool att_any=wstr_empty(att_name); //属性名が指定されていない場合は、最初に見つかったエレメントを返す。
	//※属性名が指定されている場合、属性値はNULLであってはならない。空文字列は許す。

	int len = nodes->length;
	for(int i=0;i<len;i++){
		DOMElementPtr element=nodes->item[i];
		if(element){
			//エレメント名でふるいにかける
			if(!elt_any){
				if(wcscmp(element->nodeName,elt_name)!=0)continue;
			}
				
			//属性でふるいにかける
			if(!att_any){
				_bstr_t a=element->getAttribute(att_name); //###属性が存在しないときにgetAttributeがどんな値を返すのか、未検証
				if(a!=_bstr_t(att_value))continue;
			}

			return XmlElement(element);
		}
	}
	throw ElementNotFound();
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ノード操作                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void XmlElement::remove() throw(_com_error)
{
	DOMElementPtr parent = m_pElement->parentNode;
	if(parent == NULL)
	{
		throw _com_error(E_FAIL);
	}
	
	parent->removeChild(m_pElement);
}

XmlElement XmlElement::appendChild(const bstring& tagName)
{
	MSXML2::IXMLDOMDocumentPtr pDocument = m_pElement->ownerDocument;
	DOMElementPtr pChild = pDocument->createElement(tagName);
	m_pElement->appendChild(pChild);
	
	return XmlElement(pChild);
}

XmlElement XmlElement::appendBefore(const bstring& tagName) throw(_com_error)
{
	DOMElementPtr parent = m_pElement->parentNode;
	if(parent == NULL)
	{
		throw _com_error(E_FAIL);
	}
	
	MSXML2::IXMLDOMDocumentPtr document = m_pElement->ownerDocument;
	DOMElementPtr child = document->createElement(tagName);
	parent->insertBefore(child, _variant_t(m_pElement, true));
	
	return XmlElement(child);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       実装アクセス                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

XmlDocument XmlElement::getDocument()
{
	MSXML2::IXMLDOMDocumentPtr document = m_pElement->ownerDocument;
	return XmlDocument(document);
}
