#pragma once

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

class CXmlNode;

// libxml2�h�L�������g�̃��b�p�[ //
class CXmlDocument{
private:
	// �R�s�[�s�� //
	CXmlDocument(const CXmlDocument& rhs);

public:
	// �������ƏI�� //
	CXmlDocument();
	~CXmlDocument();
	void	Free();
	bool	LoadFromFile(const char* szPath);
	bool	LoadFromString(const char* szUtf8String);

	// ��� //
	bool	IsValid() const	{ return m_pDocument != NULL; }

	// �q�擾 //
	CXmlNode	GetRootElement() const;

private:
	xmlDocPtr	m_pDocument;
};
