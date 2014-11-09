#include "_required.h"
#include <shlwapi.h>
#include <tchar.h>
#include "XmlElement.h"
#include "XmlElementList.h"
#include "XmlDocument.h"
#include <StringLib.h>

typedef _bstr_t bstring;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! �������Ȃ��B
XmlElement::XmlElement()
{
}

//! IXMLDOMElementPtr �����b�v����B
XmlElement::XmlElement(DOMElementPtr& pElement)
: m_pElement(pElement)
{
}

XmlElement::~XmlElement()
{
	m_pElement=NULL;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        �����̎擾                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! �G�������g�����擾
bstring XmlElement::getTagName() const
{
	return m_pElement->nodeName;
}

//! �������擾
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

//!< �������X�g���擾
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

//! �e�L�X�g�̔z����擾
/*!
��F<tag1>text1<tag2>text2</tag2>test3</tag1> �̂Ƃ��� 
tag1.getTextArray()�� text1, text3 �ɂȂ�
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
//                        �����̐ݒ�                           //
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

//! �e�L�X�g�̔z���ݒ�
/*!
��F<tag1><tag2></tag2><tag2></tag2></tag1> �̂Ƃ���
tag1.setText(text1, text2)�� <tag1>text1<tag2></tag2>text2</tag1> �ɂȂ�
�z�񒆂ɋ󕶎�������Ƃ��́A�ݒ肹���ɂƂ΂�
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

//! �������폜
void XmlElement::removeAttribute(const bstring& bszName)
{
	m_pElement->removeAttribute(bszName);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        �m�[�h�擾                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

XmlElement XmlElement::getParent() throw(_com_error)
{
	// �e��ElementNode��DocumentNode�i���[�g�^�O�̏ꍇ�j�̂ǂ��炩
	// NULL�̂Ƃ��͂Ȃ�
	DOMElementPtr	pParent=m_pElement->parentNode;
	// QueryInterface���������������ׂ�
	if(!pParent)
	{
		throw _com_error(E_FAIL);
	}
	return XmlElement(pParent);
}


//! �q���̃^�O���擾
/*!
�q���̃^�O�̂���tagName�Ɏw�肳�ꂽ�^�O���̂���(tagName���擾���Ȃ��Ƃ���
���ׂĂ̎q���̃^�O)�ōŏ��̂��̂��擾����
�Y������^�O���Ȃ��Ƃ���_com_error����������B
*/
XmlElement XmlElement::child(const bstring& tagName) throw(_com_error)
{
	MSXML2::IXMLDOMNodeListPtr nodes = m_pElement->childNodes;
	
	bool any=(tagName.length()==0); //���O���w�肳��Ă��Ȃ��ꍇ�́A�ŏ��Ɍ��������G�������g��Ԃ��B

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

//! �q���̃^�O���擾
/*!
�q���̃^�O�̂���tagName�Ɏw�肳�ꂽ�^�O���̂���(tagName���擾���Ȃ��Ƃ���
���ׂĂ̎q���̃^�O)���擾����
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



//! 1�̑��������Ŏq�G�������g��T���B
XmlElement XmlElement::findChild(const wchar_t* elt_name,const wchar_t* att_name,const wchar_t* att_value)
{
	MSXML2::IXMLDOMNodeListPtr nodes = m_pElement->childNodes;
	
	bool elt_any=wstr_empty(elt_name); //���O���w�肳��Ă��Ȃ��ꍇ�́A�ŏ��Ɍ��������G�������g��Ԃ��B
	bool att_any=wstr_empty(att_name); //���������w�肳��Ă��Ȃ��ꍇ�́A�ŏ��Ɍ��������G�������g��Ԃ��B
	//�����������w�肳��Ă���ꍇ�A�����l��NULL�ł����Ă͂Ȃ�Ȃ��B�󕶎���͋����B

	int len = nodes->length;
	for(int i=0;i<len;i++){
		DOMElementPtr element=nodes->item[i];
		if(element){
			//�G�������g���łӂ邢�ɂ�����
			if(!elt_any){
				if(wcscmp(element->nodeName,elt_name)!=0)continue;
			}
				
			//�����łӂ邢�ɂ�����
			if(!att_any){
				_bstr_t a=element->getAttribute(att_name); //###���������݂��Ȃ��Ƃ���getAttribute���ǂ�Ȓl��Ԃ��̂��A������
				if(a!=_bstr_t(att_value))continue;
			}

			return XmlElement(element);
		}
	}
	throw ElementNotFound();
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        �m�[�h����                           //
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
//                       �����A�N�Z�X                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

XmlDocument XmlElement::getDocument()
{
	MSXML2::IXMLDOMDocumentPtr document = m_pElement->ownerDocument;
	return XmlDocument(document);
}
