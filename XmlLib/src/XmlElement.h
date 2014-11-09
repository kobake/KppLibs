#pragma once

#include "_head.h"
#include <map>
#include <vector>

class XmlElementList;
class XmlDocument;

#include "std/myexception.h"

//! IXMLDOMElementPtrのラッパー
class XmlElement{
public:
	//例外
	GEN_EXCEPTION(ElementNotFound,L"ElementNotFound");
	GEN_EXCEPTION(AttributeNotFound,L"AttributeNotFound");

public:
	friend XmlElementList;

private:
	typedef MSXML2::IXMLDOMElementPtr DOMElementPtr;

public:
	//コンストラクタ・デストラクタ
	XmlElement();                        //!< 何もしない。
	XmlElement(DOMElementPtr& pElement); //!< IXMLDOMElementPtr をラップする。
	virtual ~XmlElement();

	//属性の取得
	_bstr_t	getTagName() const;                    //!< エレメント名を取得
	_bstr_t	getAttribute(const _bstr_t& bszName);  //!< 属性を取得。存在しない場合は例外AttributeNotFoundを投げる。
	std::map<_bstr_t, _bstr_t> getAttributes();    //!< 属性リストを取得
	_bstr_t getText();                             //!< テキストを取得
	std::vector<_bstr_t> getTextArray();           //!< テキストの配列を取得。詳細はソース側コメント参照。

	// 属性の設定
	void setAttribute(const _bstr_t& bszName, const _bstr_t& bszValue); //!< 属性を設定
	void setText(const _bstr_t& bszText);                               //!< テキストを設定
	void setTextArray(std::vector<_bstr_t> &textArray);                 //!< テキストの配列を設定。詳細はソース側コメントを参照。
	void removeAttribute(const _bstr_t& bszName);                       //!< 属性を削除

	//ノード取得
	XmlElement getParent() throw(_com_error);                                  //!< 親エレメントを取得。親が存在しない場合は例外をスロー。
	XmlElement child(const _bstr_t& tagName = (LPWSTR)NULL) throw(_com_error); //!< 子エレメントを取得。タグ名で検索する。
	XmlElementList childList(const _bstr_t& tagName = (LPWSTR)NULL);           //!< 子エレメント群を取得
	XmlElement findChild(const wchar_t* elt_name,const wchar_t* att_name,const wchar_t* att_value);  //!< 1つの属性条件で子エレメントを探す。
	XmlElement findChild(const wchar_t* element_name,const wchar_t* condition);    //!< 任意条件で子エレメントを探す。条件は"att1=a && att2=c"のように指定できる。(###未実装)

	//ノード操作
	void remove() throw(_com_error);                                   //!< このタグを削除、削除後はこのタグに対して操作を行ってはいけない
	XmlElement appendChild(const _bstr_t& tagName);                    //!< 子エレメントを追加。既に子エレメントを持つ場合は、最後尾に追加。
	XmlElement appendBefore(const _bstr_t& tagName) throw(_com_error); //!< 兄弟エレメントを自分の手前に追加。追加できなかった場合は例外をスロー。

	//実装アクセス
	DOMElementPtr& impGet(){ return m_pElement; }                //!< IXMLDOMElementPtrを取得
	void impSet(DOMElementPtr& pElement){ m_pElement=pElement; } //!< IXMLDOMElementPtrを設定
	XmlDocument getDocument();                                   //!< XmlDocumentインスタンスを取得
	bool valid() const{ return m_pElement!=NULL; }

private:
	// ラップするXMLElement
	DOMElementPtr m_pElement;
};
