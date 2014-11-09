#include "_required.h"
#include "CFtp.h"
#include "std/myexception.h"
#include "CIpAddress.h"
using namespace std;


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
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
//                           �ڑ�                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CFtp::Connect(const CIpAddress& ip,const wchar_t* user,const wchar_t* pass)
{
	Disconnect();

	//�C���^�[�l�b�g�ɐڑ�
	m_hInet=InternetOpen(
		L"MofmofFtp", //�G�[�W�F���g��
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0
	);
	if(m_hInet==NULL){
		throw wexception(L"InternetOpen failed");
	}

	//FTP�T�[�o�ɐڑ�
	m_hHost=InternetConnect(
		m_hInet,
		ip.to_string(), //�T�[�o
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
//                   �ړ��C���^�[�t�F�[�X                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! FTP�J�����g�f�B���N�g����ݒ�
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
//                   �擾�C���^�[�t�F�[�X                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! FTP�J�����g�f�B���N�g�����̃A�C�e���ꗗ���擾
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

//! ftpfile����localfile�փ_�E�����[�h
void CFtp::Download(const wchar_t* ftpfile,const wchar_t* localfile)
{
	BOOL ret=FtpGetFile(
		m_hHost,
		ftpfile,   // �_�E�����[�h���t�@�C����
		localfile, // �_�E�����[�h��t�@�C����
		FALSE,     // FALSE:���[�J�����㏑������
		FILE_ATTRIBUTE_NORMAL,    //
		FTP_TRANSFER_TYPE_BINARY, //
		0
	);
	if(!ret){
		throw wexception(L"CFtp::download failed");
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                   �����C���^�[�t�F�[�X                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! ���O�ύX
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

//! localfile����ftpfile�փA�b�v���[�h
void CFtp::Upload(const wchar_t* localfile,const wchar_t* ftpfile)
{
	BOOL ret=FtpPutFile(
		m_hHost,
		localfile, //�A�b�v���[�h���t�@�C����
		ftpfile,   //�A�b�v���[�h��t�@�C����
		FTP_TRANSFER_TYPE_BINARY, //�o�C�i������
		0
	);
	if(!ret){
		throw wexception(L"CFtp::upload failed");
	}
}

//! �t�@�C���폜
void CFtp::Remove(const wchar_t* ftpfile)
{
	BOOL ret=FtpDeleteFile(m_hHost,ftpfile);
	if(!ret){
		throw wexception(L"CFtp::remove failed");
	}
}

//! �f�B���N�g���쐬
void CFtp::Mkdir(const wchar_t* ftpdir)
{
	BOOL ret=FtpCreateDirectory(m_hHost,ftpdir);
	if(!ret){
		throw wexception(L"CFtp::mkdir failed");
	}
}

/*
####�r��


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
