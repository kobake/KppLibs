#pragma once

class CRegName{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CRegName(const CRegKey& key,const char* value_name,bool create);
	virtual ~CRegName();
	//����
	void setString(const char* str);
};
