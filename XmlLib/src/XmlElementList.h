#pragma once

#include "_head.h"
#include <vector>
class XmlElement;

// XmlElement�̔z����Ǘ�����N���X
// �����ō쐬����̂ł͂Ȃ��AchildList()�̖߂�l���g�p����B
class XmlElementList{
public:
	friend XmlElement;

public:
	//�R���X�g���N�^�E�f�X�g���N�^
	XmlElementList(){}
	virtual ~XmlElementList(){}

	//�v�f�̎擾
	size_t size() { return _vTags.size(); } //!<�v�f��
	XmlElement& operator[](size_t index){ return _vTags[index]; }             //!<�v�f�ւ̃A�N�Z�X
	const XmlElement& operator[](size_t index) const{ return _vTags[index]; } //!<�v�f�ւ̃A�N�Z�X
	
	//�q�̎擾
	XmlElement child(const _bstr_t& bszTagName = (LPWSTR)NULL) throw(_com_error); //!<���ׂĂ̗v�f��childList�𓝍��������̂̂����A�ŏ��̗v�f��Ԃ�
	XmlElementList childList(const _bstr_t& bszTagName = (LPWSTR)NULL);           //!<���ׂĂ̗v�f��childList�𓝍����ĕԂ�
private:
	// XmlElementList�ւ̗v�f�̒ǉ�
	void add(MSXML2::IXMLDOMElementPtr &pElement);

private:
	// XmlElementList���Ǘ�����v�f
	std::vector<XmlElement> _vTags;
};
