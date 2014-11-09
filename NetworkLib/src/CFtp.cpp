#include "_required.h"
#include "CFtp.h"
#include "std/myexception.h"
#include "CIpAddress.h"
using namespace std;


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CFtp::CFtp()
: m_hInet(NULL)
, m_hHost(NULL)
{
}

CFtp::CFtp(const CIpAddress& ip,const wchar_t* user,const wchar_t* pass)
: m_hInet(NULL)
, m_hHost(NULL)
{
	Connect(ip, user, pass);
}

CFtp::~CFtp()
{
	Disconnect();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           接続                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CFtp::Connect(const CIpAddress& ip,const wchar_t* user,const wchar_t* pass)
{
	Disconnect();

	//インターネットに接続
	m_hInet=InternetOpen(
		L"MofmofFtp", //エージェント名
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0
	);
	if(m_hInet==NULL){
		throw wexception(L"InternetOpen failed");
	}

	//FTPサーバに接続
	m_hHost=InternetConnect(
		m_hInet,
		ip.to_string(), //サーバ
		INTERNET_INVALID_PORT_NUMBER,
		user,
		pass,
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_PASSIVE,
		0
	);
	if(m_hHost==NULL){
		throw wexception(L"InternetConnect failed");
	}
}

void CFtp::Disconnect()
{
	if(m_hHost){
		InternetCloseHandle(m_hHost);
		m_hHost = NULL;
	}
	if(m_hInet){
		InternetCloseHandle(m_hInet);
		m_hInet = NULL;
	}
}

bool CFtp::IsConnected() const
{
	return m_hHost!=NULL;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                   移動インターフェース                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! FTPカレントディレクトリを設定
bool CFtp::Chdir(const wchar_t* dir)
{
	BOOL ret=FtpSetCurrentDirectory(m_hHost,dir);
	return ret!=FALSE;
}

std::wstring CFtp::GetCurrentDir() const
{
	wchar_t buf[512] = L""; //###
	DWORD dwSize = _countof(buf)-1;
	::FtpGetCurrentDirectory(m_hHost, buf, &dwSize);
	return buf;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                   取得インターフェース                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! FTPカレントディレクトリ内のアイテム一覧を取得
std::vector<WIN32_FIND_DATA> CFtp::Ls(const wchar_t* wildcard)
{
	vector<WIN32_FIND_DATA> ret;

	WIN32_FIND_DATA fd;
	HINTERNET hFind=FtpFindFirstFile(m_hHost,wildcard,&fd,INTERNET_FLAG_DONT_CACHE,0);
	if(hFind){
		do{
			ret.push_back(fd);
		}while(InternetFindNextFile(hFind,&fd));
	}
	InternetCloseHandle(hFind);

	return ret;
}

//! ftpfileからlocalfileへダウンロード
void CFtp::Download(const wchar_t* ftpfile,const wchar_t* localfile)
{
	BOOL ret=FtpGetFile(
		m_hHost,
		ftpfile,   // ダウンロード元ファイル名
		localfile, // ダウンロード先ファイル名
		FALSE,     // FALSE:ローカルを上書きする
		FILE_ATTRIBUTE_NORMAL,    //
		FTP_TRANSFER_TYPE_BINARY, //
		0
	);
	if(!ret){
		throw wexception(L"CFtp::download failed");
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                   書込インターフェース                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! 名前変更
void CFtp::Rename(const wchar_t* szOldName, const wchar_t* szNewName)
{
	BOOL ret = ::FtpRenameFile(
		m_hHost,
		szOldName,
		szNewName
	);
	if(!ret){
		throw wexception(L"CFtp::Rename failed.");
	}
}

//! localfileからftpfileへアップロード
void CFtp::Upload(const wchar_t* localfile,const wchar_t* ftpfile)
{
	BOOL ret=FtpPutFile(
		m_hHost,
		localfile, //アップロード元ファイル名
		ftpfile,   //アップロード先ファイル名
		FTP_TRANSFER_TYPE_BINARY, //バイナリ扱い
		0
	);
	if(!ret){
		throw wexception(L"CFtp::upload failed");
	}
}

//! ファイル削除
void CFtp::Remove(const wchar_t* ftpfile)
{
	BOOL ret=FtpDeleteFile(m_hHost,ftpfile);
	if(!ret){
		throw wexception(L"CFtp::remove failed");
	}
}

//! ディレクトリ作成
void CFtp::Mkdir(const wchar_t* ftpdir)
{
	BOOL ret=FtpCreateDirectory(m_hHost,ftpdir);
	if(!ret){
		throw wexception(L"CFtp::mkdir failed");
	}
}

/*
####途中


#if 0
int main()
{
	FTPADDRESS ftp={"ftp.geocities.jp","/ke_1k/"};
	ACCOUNT account={"ke_1k","ahehoporo"};
	if(my_connect(ftp,account)){
		ftp_dir("/ke_1k/");
		ftp_mkdir("tt2");
		ftp_dir("tt2");
		ftp_upload("c.map");
		ftp_dir("..");
		ftp_upload("a.map");
//		ftp_delete("a.gif");
//		upload("a.gif");
//		download("a.gif");
//		my_ls("*.jpg");
//		download("junk1.jpg");
	}
	InternetCloseHandle(m_hHost);
	InternetCloseHandle(m_hInet);
	
}

#endif

*/
