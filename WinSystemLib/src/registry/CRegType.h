#pragma once

class CRegType{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CRegType(DWORD _type);
	CRegType(const wchar_t* _type_name);
	//����
	DWORD getDword() const{ return type; }
private:
	DWORD type;
};
