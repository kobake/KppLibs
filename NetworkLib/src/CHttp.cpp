#include "_required.h"
#include "CHttp.h"
#include <vector>
#include <string>
using namespace std;


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CHttp::CHttp(const wchar_t* url)
: m_hInet(NULL)
, m_hUrl(NULL)
{
	try{
		//�C���^�[�l�b�g�J�n
		m_hInet=InternetOpen(
			L"MofmofBrowser",
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL,
			NULL,
			0
		);
		if(m_hInet==NULL){
			throw wexception(L"InternetOpen failed");
		}

		//URL���w�肳��Ă���΁AGET����
		if(url){
			this->RequestGet(url, TParams());
		}
	}
	catch(...){
		_final();
		throw;
	}
}

CHttp::~CHttp()
{
	_final();
}

void CHttp::_final()
{
	if(m_hUrl){ InternetCloseHandle(m_hUrl); m_hUrl=NULL; }
	if(m_hInet){ InternetCloseHandle(m_hInet); m_hInet=NULL; }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ���N�G�X�g                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CHttp::RequestGet(const wchar_t* szUrl, const TParams& tParams)
{
	if(m_hUrl){ InternetCloseHandle(m_hUrl); m_hUrl = NULL; }

	//HTTP�Z�b�V�����̊J�n�A�w���URL�I�[�v��
	DWORD dwFlags = 0
		| INTERNET_FLAG_NO_AUTO_REDIRECT	// �������_�C���N�g //
		| INTERNET_FLAG_RELOAD;				// ���[�J���L���b�V������ //
	m_hUrl=InternetOpenUrl(
		m_hInet,
		szUrl,
		NULL,			//[in] ���N�G�X�g�w�b�_
		0,				//[in] ���N�G�X�g�w�b�_��
		dwFlags,		//[in] �e��t���O
		0				//[in] �A�v����`�̒l (�R�[���o�b�N�p)
	);

	if(m_hUrl==NULL){
		InternetCloseHandle(m_hInet);
		throw wexception(L"InternetOpenUrl failed");
	}
}

//! POST���N�G�X�g
/*
void CHttp::RequestPost(const wchar_t* szUrl, const TParams& tParams)
{
	if(m_hUrl){ InternetCloseHandle(m_hUrl); m_hUrl = NULL; }

	CHttpRequest cReuqest(
	::InternetConnect(m_hInet, L"
	::HttpOpenRequest(
		m_hInet,
		L"POST",
		L"/page/a.html",
		NULL,	//version
		NULL,	//referer
		NULL,	//accept_types
		INTERNET_FLAG_RELOAD,	//flag
		0						//dwContext
	);

	//HTTP�Z�b�V�����̊J�n�A�w���URL��POST
	::HttpSendRequest(
		m_hInet,	//HINTERNET
		strHeaders,	//�w�b�_
		nHeaders,	//�w�b�_��
		option,
		optionlen
	);


	m_hUrl=InternetOpenUrl(
		m_hInet,
		url,
		NULL,								//[in] ���N�G�X�g�w�b�_
		0,									//[in] ���N�G�X�g�w�b�_��
		INTERNET_FLAG_NO_AUTO_REDIRECT,		//[in] �e��t���O
		0									//[in] �A�v����`�̒l (�R�[���o�b�N�p)
	);

	if(m_hUrl==NULL){
		InternetCloseHandle(m_hInet);
		throw wexception(L"InternetOpenUrl failed");
	}
}
*/

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �C���^�[�t�F�[�X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

std::wstring CHttp::GetHeader() const
{
	DWORD dwSize=255;
	vector<wchar_t> buf(dwSize+1);
	BOOL b;

retry:
	b=HttpQueryInfoW(
		m_hUrl,
		HTTP_QUERY_RAW_HEADERS_CRLF,
		&buf[0],
		&dwSize,
		NULL
	);

	if(!b){
		switch(GetLastError()){
		case ERROR_HTTP_HEADER_NOT_FOUND:
			//�w�b�_�������̂ŋ󕶎����Ԃ�
			return L"";
		case ERROR_INSUFFICIENT_BUFFER:
			//���������Ċm�ۂ��ă��g���C
			buf.resize(dwSize+1);
			goto retry;
		default:
			//����ȊO�̃G���[�ł͗�O�𓊂���
			throw wexception(L"HttpQueryInfo failed");
		}
	}

	buf[dwSize]=L'\0';
	return &buf[0];
}

//! �����R�[�h
EHttpStatusCode CHttp::GetStatusCode() const
{
	wchar_t buf[256];
	DWORD dwSize = sizeof(buf)-1;
	BOOL b =HttpQueryInfoW(
		m_hUrl,
		HTTP_QUERY_STATUS_CODE,
		buf,
		&dwSize,
		NULL
	);
	if(b){
		buf[dwSize] = L'\0';
		return (EHttpStatusCode)_wtoi(buf);
	}
	else{
		throw exception("HttpQueryInfo(HTTP_QUERY_STATUS_CODE) failed.");
	}
}

#include <StringLib.h>

std::string CHttp::ReadAllA() const
{
	//�ǂݏo�����̂��Ȃ��Ȃ�܂œǂݏo��
	vector<char> ret(0);
	while(1){
		DWORD dwRead;
		vector<char> tmp(256);
		InternetReadFile(
			m_hUrl,
			&tmp[0],
			(DWORD)tmp.size(),
			&dwRead
		);

		//�ǂݏo�����̂��Ȃ��Ȃ����烋�[�v�E�o
		if(dwRead==0)break;

		//ret�Ɍ���
		ret.insert(ret.end(),tmp.begin(),tmp.begin()+dwRead);
	}
	ret.push_back('\0');

	return &ret[0];
}

std::wstring CHttp::ReadAll(bool bUtf8) const
{
	//�ǂݏo�����̂��Ȃ��Ȃ�܂œǂݏo��
	vector<char> ret(0);
	while(1){
		DWORD dwRead;
		vector<char> tmp(256);
		InternetReadFile(
			m_hUrl,
			&tmp[0],
			(DWORD)tmp.size(),
			&dwRead
		);

		//�ǂݏo�����̂��Ȃ��Ȃ����烋�[�v�E�o
		if(dwRead==0)break;

		//ret�Ɍ���
		ret.insert(ret.end(),tmp.begin(),tmp.begin()+dwRead);
	}
	ret.push_back('\0');

	//�����R�[�h�ϊ� UTF-8��UNICODE (####��)
	if(bUtf8 || strstr(&ret[0], "charset=utf-8")){
		const char* p = &ret[0];
		return cpp_mbstowcs(p, CP_UTF8);
		/*
		int new_size = ::MultiByteToWideChar(CP_UTF8, 0, p, strlen(p), NULL, 0);
		vector<wchar_t>
		int new_size = UTF8NtoSJIS(p,NULL,0);
		vector<char> ret2(new_size+1);
		new_size = UTF8NtoSJIS(p, &ret2[0], new_size);
		return cpp_mbstowcs(&ret2[0]);
		*/
	}

	//������ɕϊ�
	return cpp_mbstowcs(&ret[0]);
}

#include "CDataSocket.h"

//GET���N�G�X�g�̕ԓ����擾����
std::string CHttp::RequestGet(const wchar_t* server,const wchar_t* url) const
{
	//�\�P�b�g���J��
	CSocket _s;
	CDataSocket s(&_s);

	//�T�[�o�ɐڑ�
	CIpAddress ip(server); //IP
	CSocketAddress addr(ip, 80); //IP+�|�[�g
	s.Connect(addr);

	//GET���N�G�X�g
	int ret=s.sendf("GET %ls\n",url);

	//��M
	string rc=s.recv_string();

	return rc;
}

