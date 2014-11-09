#pragma once

#include "CRegHive.h"
#include "CRegKey.h"
#include <string>


class CRegPath{
public:
	CRegPath(const wchar_t* path,bool to_create);
	CRegKey& getKey(){ return key; }
	const wchar_t* getValueName() const{ return value_name.c_str(); }
private:
	CRegHive hive;
	CRegKey key;
	std::wstring value_name;
};
