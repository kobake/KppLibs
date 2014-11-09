#pragma once

class CRegName{
public:
	//コンストラクタ・デストラクタ
	CRegName(const CRegKey& key,const char* value_name,bool create);
	virtual ~CRegName();
	//動作
	void setString(const char* str);
};
