#include "CXmlDocument.h"
#include "CXmlNode.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �������ƏI��
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
// �q�擾
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CXmlNode CXmlDocument::GetRootElement() const
{
	if(!m_pDocument)return CXmlNode(NULL);
	xmlNodePtr cur = xmlDocGetRootElement(m_pDocument); // �������͊m�ۂ����ɁA���ɂ����Ă�����ւ̃|�C���^��Ԃ��炵�� //
	return CXmlNode(cur);
}
