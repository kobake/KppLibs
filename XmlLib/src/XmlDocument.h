#pragma once

#include "_head.h"
class XmlElement;

// XMLDocument�̃��b�p�[
class XmlDocument{
private:
	typedef MSXML2::IXMLDOMDocumentPtr DOMDocPtr;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	XmlDocument();                                    //!< �������Ȃ��B
	XmlDocument(DOMDocPtr& pDocument);                //!< IXMLDOMDocumentPtr�����b�v����B
	explicit XmlDocument(const _bstr_t& rootTagName); //!< ���[�g�^�O�����w�肵�A�V����XMLDocument���쐬����B
	virtual ~XmlDocument();

	//����
	static XmlDocument load(const _bstr_t& uri); //!< �t�@�C������ǂݎ��
	static XmlDocument load(IStream* stream);    //!< �X�g���[������ǂݎ��

	//�o��
	void save(const _bstr_t& filename);   //!< �t�@�C���֏�������
	void save(IStream* stream);           //!< �X�g���[���֏�������
	void saveEx(const _bstr_t& filename); //!< �t�@�C���֏�������

	//�m�[�h
	XmlElement root(); //!< XmlDocument�̃��[�g�G�������g���擾

	//�����A�N�Z�X
	void impSet(DOMDocPtr& pDocument){ m_pDocument = pDocument; } //!< IXMLDOMDocumentPtr��ݒ�
	DOMDocPtr& impGet(){ return m_pDocument; }                    //!< IXMLDOMDocumentPtr���擾

private:
	DOMDocPtr m_pDocument; //!< ���b�v����XMLDocument
};
