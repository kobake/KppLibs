#pragma once

#include <boost/shared_ptr.hpp>

class CDbRecord{
public:
	//�������ƏI��
	CDbRecord();
	~CDbRecord();
	//�擾
	int Count() const{ return m_pRs->RecordCount; }			//!< ���R�[�h���擾
	_bstr_t GetValue(const wchar_t* szColumnName) const;	//!< SQL���ʎ擾
	void Next();											//!< ����SQL���ʂ֐i��
	bool Eof() const;										//!< SQL���ʎ擾���I�������ǂ���

public:
	//�����⏕
	void _Query(const wchar_t* z, _ConnectionPtr pCn);	//!< SQL���s
	_RecordsetPtr _GetPtr() const{ return m_pRs; }		//!< �����A�N�Z�X
private:
	_RecordsetPtr	m_pRs;
	bool			m_bOpened;
};
