#pragma once

class CRegType{
public:
	//コンストラクタ・デストラクタ
	CRegType(DWORD _type);
	CRegType(const wchar_t* _type_name);
	//動作
	DWORD getDword() const{ return type; }
private:
	DWORD type;
};
