#pragma once

#include <vector>
#include <string>
#include <wininet.h> //HINTERNET
#include <winsock2.h> //windows.hより前にないとダメ
#include <windows.h> //WIN32_FIND_DATA

class CIpAddress;

class CFtp{
public:
	//コンストラクタ・デストラクタ
	CFtp();
	CFtp(const CIpAddress& ip,const wchar_t* user,const wchar_t* pass);
	~CFtp();
	//接続
	void Connect(const CIpAddress& ip,const wchar_t* user,const wchar_t* pass);
	void Disconnect();
	bool IsConnected() const;
	//移動インターフェース
	bool							Chdir(const wchar_t* dir);			//!< FTPカレントディレクトリを設定
	std::wstring					GetCurrentDir() const;
	//取得インターフェース
	std::vector<WIN32_FIND_DATA>	Ls(const wchar_t* wildcard);		//!< FTPカレントディレクトリ内のアイテム一覧を取得
	void							Download(const wchar_t* ftpfile,const wchar_t* localfile);	//!< ftpfileからlocalfileへダウンロード
	//書込インターフェース
	void Rename(const wchar_t* szOldName, const wchar_t* szNewName);	//!< 名前変更
	void Upload(const wchar_t* localfile,const wchar_t* ftpfile);		//!< localfileからftpfileへアップロード
	void Remove(const wchar_t* ftpfile);								//!< ファイル削除
	void Mkdir(const wchar_t* ftpdir);									//!< ディレクトリ作成
private:
	HINTERNET m_hInet;
	HINTERNET m_hHost;
};

#include "_lib_wininet.h"





