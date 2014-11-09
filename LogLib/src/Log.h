#pragma once

#include <string>

//### Logをメンバ変数に持つクラスがconst関数からでもprintを呼べるように、
//### printをconst関数にしてるが、これは正しい？？

//! ログ出力クラス
class Log{
public:
	//コンストラクタ・デストラクタ
	Log() : enabled(true) { }
	virtual ~Log(){}

	//スイッチ
	void setEnabled(bool b){ enabled=b; }
	bool isEnabled() const{ return enabled; }

	//出力
	void print(const wchar_t* str, ...) const;
	void print(const std::string& str) const;
	void print(const std::wstring& str) const;
private:
	bool enabled;
};
