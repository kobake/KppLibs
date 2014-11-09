#pragma once

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

class CXmlNode;

// libxml2ドキュメントのラッパー //
class CXmlDocument{
private:
	// コピー不可 //
	CXmlDocument(const CXmlDocument& rhs);

public:
	// 初期化と終了 //
	CXmlDocument();
	~CXmlDocument();
	void	Free();
	bool	LoadFromFile(const char* szPath);
	bool	LoadFromString(const char* szUtf8String);

	// 状態 //
	bool	IsValid() const	{ return m_pDocument != NULL; }

	// 子取得 //
	CXmlNode	GetRootElement() const;

private:
	xmlDocPtr	m_pDocument;
};
