#pragma once

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>

// libxml2�m�[�h�̃��b�p�[�B���ɉ�������͂��Ȃ� //
class CXmlNode{
public:
	// �������ƏI�� //
	explicit CXmlNode(xmlNodePtr p) : m_pNode(p) {}

	// ��� //
	bool			IsValid() const	{ return m_pNode != NULL; }

	// ������ //
	const char*		GetName() const;
	std::wstring	GetProperty(const char* szName) const;
	std::wstring	GetText() const;

	// �q�擾 //
	int				GetChildCount() const;
	CXmlNode		GetChild(int iIndex) const;
	CXmlNode		FindFirstChild(const char* szNodeName) const;
	CXmlNode		FindFirstTextChild() const;

private:
	xmlNodePtr	m_pNode;
};
