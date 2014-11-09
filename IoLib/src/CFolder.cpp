#include "common/_required.h"
#include "CFolder.h"

class Sorter{
public:
	int compare(const WIN32_FIND_DATA& lhs, const WIN32_FIND_DATA& rhs) const
	{
		// �f�B���N�g������ //
		bool bLhsDir = (lhs.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		bool bRhsDir = (rhs.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		int dir = (bLhsDir?0:1) - (bRhsDir?0:1);
		if(dir != 0)return dir;
		// �t�@�C���� //
		int name = wcsicmp(lhs.cFileName, rhs.cFileName);
		if(name != 0)return name;
		// ���ɍ������� //
		return 0;
	}
	bool operator()(const WIN32_FIND_DATA& lhs, const WIN32_FIND_DATA& rhs) const
	{
		return compare(lhs, rhs) < 0;
	}
};

void CFolder::EnumItems(const wchar_t* szPattern, std::vector<WIN32_FIND_DATA>* pvFiles)
{
	std::wstring strPath = m_strPath + L"\\" + szPattern;

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &fd);
	if(hFind!=INVALID_HANDLE_VALUE){
		do{
			if(wcscmp(fd.cFileName,L".")==0)continue; //"."�͏��O
			if(wcscmp(fd.cFileName,L"..")==0)continue; //".."�͏��O
			pvFiles->push_back(fd);
		}
		while(FindNextFile(hFind,&fd));
		FindClose(hFind);
	}
}

void CFolder::EnumItems(const wchar_t* szPattern, std::list<WIN32_FIND_DATA>* pvFiles)
{
	std::wstring strPath = m_strPath + L"\\" + szPattern;

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &fd);
	if(hFind!=INVALID_HANDLE_VALUE){
		do{
			if(wcscmp(fd.cFileName,L".")==0)continue; //"."�͏��O
			if(wcscmp(fd.cFileName,L"..")==0)continue; //".."�͏��O
			pvFiles->push_back(fd);
		}
		while(FindNextFile(hFind,&fd));
		FindClose(hFind);
	}

	// �\�[�g //
	pvFiles->sort( Sorter() );
}

std::vector<std::wstring> CFolder::EnumFilesPath(const wchar_t* szPattern)
{
	std::wstring strPath = m_strPath + L"\\" + szPattern;

	std::vector<std::wstring> ret;
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &fd);
	if(hFind!=INVALID_HANDLE_VALUE){
		do{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)continue; //�f�B���N�g���͏��O
			ret.push_back(m_strPath + L"\\" + fd.cFileName);
		}
		while(FindNextFile(hFind,&fd));
		FindClose(hFind);
	}
	return ret;
}

std::vector<std::wstring> CFolder::EnumChildrenName(const wchar_t* szPattern)
{
	std::wstring strPath = m_strPath + L"\\" + szPattern;

	std::vector<std::wstring> ret;
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &fd);
	if(hFind!=INVALID_HANDLE_VALUE){
		do{
			if(wcscmp(fd.cFileName,L".")==0)continue; //"."�͏��O
			if(wcscmp(fd.cFileName,L"..")==0)continue; //".."�͏��O
			ret.push_back(fd.cFileName);
		}
		while(FindNextFile(hFind,&fd));
		FindClose(hFind);
	}
	return ret;
}

int CFolder::Test()
{
	return 100;
}
