#pragma once

#include "_head.h"
#include <vector>
class XmlElement;

// XmlElementの配列を管理するクラス
// 自分で作成するのではなく、childList()の戻り値を使用する。
class XmlElementList{
public:
	friend XmlElement;

public:
	//コンストラクタ・デストラクタ
	XmlElementList(){}
	virtual ~XmlElementList(){}

	//要素の取得
	size_t size() { return _vTags.size(); } //!<要素数
	XmlElement& operator[](size_t index){ return _vTags[index]; }             //!<要素へのアクセス
	const XmlElement& operator[](size_t index) const{ return _vTags[index]; } //!<要素へのアクセス
	
	//子の取得
	XmlElement child(const _bstr_t& bszTagName = (LPWSTR)NULL) throw(_com_error); //!<すべての要素のchildListを統合したもののうち、最初の要素を返す
	XmlElementList childList(const _bstr_t& bszTagName = (LPWSTR)NULL);           //!<すべての要素のchildListを統合して返す
private:
	// XmlElementListへの要素の追加
	void add(MSXML2::IXMLDOMElementPtr &pElement);

private:
	// XmlElementListが管理する要素
	std::vector<XmlElement> _vTags;
};
