#pragma once

#include <string>

class CIntFormat{
public:
	CIntFormat(__int64 nValue) : m_nValue(nValue) { }
	std::wstring PlainBytes() const; //ナマの値をコンマ区切りで出力 (例: 123,456 Bytes)
	std::wstring EasyBytes() const;  //KB, MB, GB 等の単位に変換して出力 (例: 123.46 KBytes)
	std::wstring EasyBytes_LikeColumn() const; //カラムに表示する風
	std::wstring EasyBytes_LikeStatusBar() const; //ステータスバー風
private:
	__int64	m_nValue;
};
