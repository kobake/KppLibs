#pragma once

#include <windows.h>
#include <string>

//
// �X�^�b�N�g���[�X�̏o�͂�񋟂��܂��B
//
class CStackTrace{
public:
	CStackTrace(void);
	~CStackTrace(void);
	void		Show();
	std::string	GetInfo();

private:
	bool LoadModules(HANDLE hProcess, DWORD dwProcessId);
};
