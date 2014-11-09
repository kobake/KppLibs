#pragma once

class CRegHive{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CRegHive(const wchar_t* root_name); //��"\\"���܂܂��ꍇ�́A����ȍ~�𖳎����܂�
	CRegHive(HKEY _hRootKey);
	CRegHive();
	CRegHive(const CRegHive& rhs) : m_hKey(rhs.m_hKey) { } //�R�s�[��
	CRegHive& operator = (const CRegHive& rhs){ m_hKey=rhs.m_hKey; return *this; } //�R�s�[��
	//����
	HKEY getHandle() const{ return m_hKey; }

	//���
	const wchar_t* ToString() const;
private:
	HKEY m_hKey;
};
