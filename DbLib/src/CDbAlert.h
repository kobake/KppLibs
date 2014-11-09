#pragma once

#include "_required.h"
#include <string>

class CDbAlert{
public:
	void PrintProviderError(_ConnectionPtr pConnection) const;
	void PrintComError(const _com_error& e) const;

	//Žæ“¾
	std::string GetProviderError(_ConnectionPtr pConnection) const;
	std::string GetComError(const _com_error& e) const;
};
