#pragma once

class CInputStream;

class InputData{
public:
	virtual ~InputData(){}
	virtual void Read(CInputStream& in)=0;
	void load(const wchar_t* fname);
};
