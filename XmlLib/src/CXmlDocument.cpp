#include "CXmlDocument.h"
#include "CXmlNode.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CXmlDocument::CXmlDocument()
: m_pDocument(NULL)
{
}

CXmlDocument::~CXmlDocument()
{
	Free();
}

void CXmlDocument::Free()
{
	if(m_pDocument){
		xmlFreeDoc(m_pDocument);
		m_pDocument = NULL;
	}
}

bool CXmlDocument::LoadFromFile(const char* szPath)
{
	Free();
	m_pDocument = xmlParseFile(szPath);
	return IsValid();
}

bool CXmlDocument::LoadFromString(const char* szUtf8String)
{
	Free();
	m_pDocument = xmlParseDoc((const xmlChar*)szUtf8String);
	return IsValid();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 子取得
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CXmlNode CXmlDocument::GetRootElement() const
{
	if(!m_pDocument)return CXmlNode(NULL);
	xmlNodePtr cur = xmlDocGetRootElement(m_pDocument); // メモリは確保せずに、既にもっている情報へのポインタを返すらしい //
	return CXmlNode(cur);
}
