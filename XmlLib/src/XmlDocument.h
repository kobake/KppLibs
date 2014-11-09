#pragma once

#include "_head.h"
class XmlElement;

// XMLDocumentのラッパー
class XmlDocument{
private:
	typedef MSXML2::IXMLDOMDocumentPtr DOMDocPtr;
public:
	//コンストラクタ・デストラクタ
	XmlDocument();                                    //!< 何もしない。
	XmlDocument(DOMDocPtr& pDocument);                //!< IXMLDOMDocumentPtrをラップする。
	explicit XmlDocument(const _bstr_t& rootTagName); //!< ルートタグ名を指定し、新しいXMLDocumentを作成する。
	virtual ~XmlDocument();

	//入力
	static XmlDocument load(const _bstr_t& uri); //!< ファイルから読み取り
	static XmlDocument load(IStream* stream);    //!< ストリームから読み取り

	//出力
	void save(const _bstr_t& filename);   //!< ファイルへ書き込み
	void save(IStream* stream);           //!< ストリームへ書き込み
	void saveEx(const _bstr_t& filename); //!< ファイルへ書き込み

	//ノード
	XmlElement root(); //!< XmlDocumentのルートエレメントを取得

	//実装アクセス
	void impSet(DOMDocPtr& pDocument){ m_pDocument = pDocument; } //!< IXMLDOMDocumentPtrを設定
	DOMDocPtr& impGet(){ return m_pDocument; }                    //!< IXMLDOMDocumentPtrを取得

private:
	DOMDocPtr m_pDocument; //!< ラップするXMLDocument
};
