#pragma once

#include <string>

//### Log�������o�ϐ��Ɏ��N���X��const�֐�����ł�print���Ăׂ�悤�ɁA
//### print��const�֐��ɂ��Ă邪�A����͐������H�H

//! ���O�o�̓N���X
class Log{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Log() : enabled(true) { }
	virtual ~Log(){}

	//�X�C�b�`
	void setEnabled(bool b){ enabled=b; }
	bool isEnabled() const{ return enabled; }

	//�o��
	void print(const wchar_t* str, ...) const;
	void print(const std::string& str) const;
	void print(const std::wstring& str) const;
private:
	bool enabled;
};
