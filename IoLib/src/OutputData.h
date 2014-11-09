#pragma once

class COutputStream;

class OutputData{
public:
	virtual ~OutputData(){}
	virtual void write(COutputStream& in)=0;
	void load(const wchar_t* fname);
};
