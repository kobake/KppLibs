#pragma once

#include "mystring.h"
#include "myvector.h"

class Regex{
public:
	Regex(const mystring& pattern);
	virtual ~Regex();
	myvector<mystring>	Match(const mystring& target) const;
	bool				IsMatch(const mystring& target) const;
	mystring			Replace(const mystring& target, const mystring& after) const;
private:
	void* m_regex;
};


