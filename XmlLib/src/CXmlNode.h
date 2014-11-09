#pragma once

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>

// libxml2ノードのラッパー。特に解放処理はしない //
class CXmlNode{
public:
	// 初期化と終了 //
	explicit CXmlNode(xmlNodePtr p) : m_pNode(p) {}

	// 状態 //
	bool			IsValid() const	{ return m_pNode != NULL; }

	// 自属性 //
	const char*		GetName() const;
	std::wstring	GetProperty(const char* szName) const;
	std::wstring	GetText() const;

	// 子取得 //
	int				GetChildCount() const;
	CXmlNode		GetChild(int iIndex) const;
	CXmlNode		FindFirstChild(const char* szNodeName) const;
	CXmlNode		FindFirstTextChild() const;

private:
	xmlNodePtr	m_pNode;
};
