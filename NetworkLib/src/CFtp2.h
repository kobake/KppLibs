/*
	ftp.exeによるftp通信モジュール
*/
#pragma once

#include <string>
#include <vector>
#include "../../ThreadLib/src/CProcess2.h"
#include "../../ThreadLib/src/CPipe.h"
#include "../../ThreadLib/src/CThread2.h"

class CReadThread;

class CFtpPermission{
public:
	CFtpPermission() : m_bIsDirectory(false), m_nPermission(0) { }
	CFtpPermission(const char* szText);
	int			GetInt() const;
	std::string	GetString() const;
	bool		IsDirectory() const{ return m_bIsDirectory; }
private:
	bool	m_bIsDirectory;
	int		m_nPermission; //000～777
};

class CFtpTime{
public:
	CFtpTime() : m_time(0) { }
	CFtpTime(const char* szMonth, const char* szDay, const char* szYearOrTime);
	std::string GetString() const;
private:
	time_t	m_time;
};

class CFtpFileInfo{
public:
	CFtpFileInfo(const char* line);
	const char*				GetName() const			{ return m_strName.c_str(); }
	__int64					GetSize() const			{ return m_nSize; }
	const char*				GetOwnerUser() const	{ return m_strOwnerUser.c_str(); }
	const char*				GetOwnerGroup() const	{ return m_strOwnerGroup.c_str(); }
	const CFtpPermission&	GetPermission() const	{ return m_cPermission; }
private:
	std::string		m_strName;
	__int64			m_nSize;
	CFtpTime		m_cTime;
	std::string		m_strOwnerUser;
	std::string		m_strOwnerGroup;
	CFtpPermission	m_cPermission;
};

class CFtp2{
public:
	//初期化と終了
	CFtp2();
	~CFtp2();
	//接続
	void Connect(
		const char*	szHost,
		const char*	szUser,
		const char* szPass
	);
	void Disconnect();
	bool IsConnected() const;
	//移動インターフェース
	bool ChDir(const char* dir);
	//取得インターフェース
	std::vector<CFtpFileInfo>	Ls();
	void						Download(const char* remote_file, const char* local_file);
	//書込インターフェース
	void	Upload(const char* local_file, const char* remote_file);
	void	Rename(const char* old_name, const char* new_name);
	void	Delete(const char* remote_file);
	void	Mkdir(const char* new_dir);
private:
	CPipe						m_cPipeReadStdOut;
	CPipe						m_cPipeWriteStdIn;
	CProcess2					m_cProcess;
	CThread2Handle<CReadThread>	m_pcThread;
};
