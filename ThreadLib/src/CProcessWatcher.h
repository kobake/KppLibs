#pragma once

class CProcess;
#include <vector>

class CProcessWatcher{
public:
	std::vector<CProcess> GetProcessList() const;

};