#include "CXmlNode.h"
#include <StringLib.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// ������ //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
const char* CXmlNode::GetName() const
{
	if(!m_pNode)return "";
	return (const char*)m_pNode->name;
}

std::wstring CXmlNode::GetProperty(const char* szName) const
{
	xmlChar* p = xmlGetProp(m_pNode, (const xmlChar*)szName);
	if(!p)return L"";
	std::wstring ret = cpp_mbstowcs((const char*)p, CP_UTF8);
	xmlFree(p);
	return ret;
}

std::wstring CXmlNode::GetText() const
{
	if(!m_pNode)return L"";
	CXmlNode nodeText = FindFirstTextChild();
	if(!nodeText.m_pNode || !nodeText.m_pNode->content)return L"";
	return cpp_mbstowcs((const char*)nodeText.m_pNode->content, CP_UTF8);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �q�擾 //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
int CXmlNode::GetChildCount() const
{
	if(!m_pNode)return 0;
	return xmlChildElementCount(m_pNode);
}

CXmlNode CXmlNode::GetChild(int iIndex) const
{
	if(!m_pNode)return CXmlNode(NULL);
	xmlNodePtr p = xmlFirstElementChild(m_pNode);	// �m�ۂ͂��Ȃ��B�Q�Ƃ�Ԃ� //
	for(int i = 0; i < iIndex && p; i++){
		p = xmlNextElementSibling(p);				// �m�ۂ͂��Ȃ��B�Q�Ƃ�Ԃ� //
	}
	return CXmlNode(p);
}

CXmlNode CXmlNode::FindFirstChild(const char* szNodeName) const
{
	if(!m_pNode)return CXmlNode(NULL);
	xmlNodePtr p = xmlFirstElementChild(m_pNode);	// �m�ۂ͂��Ȃ��B�Q�Ƃ�Ԃ� //
	while(p){
		if(strcmp((const char*)p->name, szNodeName) == 0){
			return CXmlNode(p);	// �������� //
		}
		p = xmlNextElementSibling(p);				// �m�ۂ͂��Ȃ��B�Q�Ƃ�Ԃ� //
	}
	return CXmlNode(NULL);
}

CXmlNode CXmlNode::FindFirstTextChild() const
{
	if(!m_pNode)return CXmlNode(NULL);
	xmlNodePtr p = m_pNode->children;
	while(p){
		if(p->type == XML_TEXT_NODE){
			return CXmlNode(p);	// �������� //
		}
		p = p->next;
	}
	return CXmlNode(NULL);
}
