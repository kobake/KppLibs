#pragma once

#include <string>
#include <vector>
#include <IoLib.h> // CFilePathWin

class CFileShell{
public:
	CFileShell(const CFilePathWin& cPath) : m_cPath(cPath) { }
	void Open();
	//void Delete();
	//void Cut();
	//void Copy();
	void Paste();
	void RenameTo(const CFilePathWin& cNewPath);
	//オプション
	void Property();
private:
	CFilePathWin m_cPath;
};

class CFilesShell{
public:
	CFilesShell(const std::vector<CFilePathWin> vFiles) : m_vFiles(vFiles) { }
	void MoveToDir(const CFilePathWin& cDir);
	void CreateShortcutToDir(const CFilePathWin& cDir);
	void CopyToDir(const CFilePathWin& cDir);
	//
	void Delete(bool bConfirmDialog);
	void Cut();
	void Copy();
	//オプション
	void Property();
private:
	std::vector<CFilePathWin> m_vFiles;
};

