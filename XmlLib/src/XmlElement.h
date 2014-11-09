#pragma once

#include "_head.h"
#include <map>
#include <vector>

class XmlElementList;
class XmlDocument;

#include "std/myexception.h"

//! IXMLDOMElementPtr�̃��b�p�[
class XmlElement{
public:
	//��O
	GEN_EXCEPTION(ElementNotFound,L"ElementNotFound");
	GEN_EXCEPTION(AttributeNotFound,L"AttributeNotFound");

public:
	friend XmlElementList;

private:
	typedef MSXML2::IXMLDOMElementPtr DOMElementPtr;

public:
	//�R���X�g���N�^�E�f�X�g���N�^
	XmlElement();                        //!< �������Ȃ��B
	XmlElement(DOMElementPtr& pElement); //!< IXMLDOMElementPtr �����b�v����B
	virtual ~XmlElement();

	//�����̎擾
	_bstr_t	getTagName() const;                    //!< �G�������g�����擾
	_bstr_t	getAttribute(const _bstr_t& bszName);  //!< �������擾�B���݂��Ȃ��ꍇ�͗�OAttributeNotFound�𓊂���B
	std::map<_bstr_t, _bstr_t> getAttributes();    //!< �������X�g���擾
	_bstr_t getText();                             //!< �e�L�X�g���擾
	std::vector<_bstr_t> getTextArray();           //!< �e�L�X�g�̔z����擾�B�ڍׂ̓\�[�X���R�����g�Q�ƁB

	// �����̐ݒ�
	void setAttribute(const _bstr_t& bszName, const _bstr_t& bszValue); //!< ������ݒ�
	void setText(const _bstr_t& bszText);                               //!< �e�L�X�g��ݒ�
	void setTextArray(std::vector<_bstr_t> &textArray);                 //!< �e�L�X�g�̔z���ݒ�B�ڍׂ̓\�[�X���R�����g���Q�ƁB
	void removeAttribute(const _bstr_t& bszName);                       //!< �������폜

	//�m�[�h�擾
	XmlElement getParent() throw(_com_error);                                  //!< �e�G�������g���擾�B�e�����݂��Ȃ��ꍇ�͗�O���X���[�B
	XmlElement child(const _bstr_t& tagName = (LPWSTR)NULL) throw(_com_error); //!< �q�G�������g���擾�B�^�O���Ō�������B
	XmlElementList childList(const _bstr_t& tagName = (LPWSTR)NULL);           //!< �q�G�������g�Q���擾
	XmlElement findChild(const wchar_t* elt_name,const wchar_t* att_name,const wchar_t* att_value);  //!< 1�̑��������Ŏq�G�������g��T���B
	XmlElement findChild(const wchar_t* element_name,const wchar_t* condition);    //!< �C�ӏ����Ŏq�G�������g��T���B������"att1=a && att2=c"�̂悤�Ɏw��ł���B(###������)

	//�m�[�h����
	void remove() throw(_com_error);                                   //!< ���̃^�O���폜�A�폜��͂��̃^�O�ɑ΂��đ�����s���Ă͂����Ȃ�
	XmlElement appendChild(const _bstr_t& tagName);                    //!< �q�G�������g��ǉ��B���Ɏq�G�������g�����ꍇ�́A�Ō���ɒǉ��B
	XmlElement appendBefore(const _bstr_t& tagName) throw(_com_error); //!< �Z��G�������g�������̎�O�ɒǉ��B�ǉ��ł��Ȃ������ꍇ�͗�O���X���[�B

	//�����A�N�Z�X
	DOMElementPtr& impGet(){ return m_pElement; }                //!< IXMLDOMElementPtr���擾
	void impSet(DOMElementPtr& pElement){ m_pElement=pElement; } //!< IXMLDOMElementPtr��ݒ�
	XmlDocument getDocument();                                   //!< XmlDocument�C���X�^���X���擾
	bool valid() const{ return m_pElement!=NULL; }

private:
	// ���b�v����XMLElement
	DOMElementPtr m_pElement;
};
