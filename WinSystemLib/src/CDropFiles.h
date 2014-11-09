#pragma once

#include <windows.h>
#include <shellapi.h> // HDROP

class CDropFiles{
public:
	CDropFiles(HDROP _hDrop);
	virtual ~CDropFiles();
	void finish();
	size_t size() const;
	const wchar_t* GetPath(size_t index) const;
private:
	HDROP hDrop;
	size_t num;
};


#include <vector>
#include <string>
std::vector<std::wstring> GetDropFiles(HDROP hDrop);
