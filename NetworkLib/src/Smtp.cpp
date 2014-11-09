#include "_required.h"
#include "Smtp.h"
#include "CIpAddress.h"
#include "Mail.h"
#include "CSocketAddress.h"
using namespace std;


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Smtp::Smtp(bool _log_print)
: s(&s2)
{
	log.setEnabled(_log_print);
}

Smtp::~Smtp()
{
	quit();
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �C���^�[�t�F�[�X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!< �ڑ�
void Smtp::Connect(const wchar_t* _hostname)
{
	hostname=_hostname;

	//�ڑ�
	CIpAddress ip(hostname.c_str());
	CSocketAddress addr(ip,Mail::getSmtpPort());
	s.Connect(addr);
}

void Smtp::quit()
{
	if(s.valid()){
		string buf;
		s.sendf("QUIT\r\n"); buf=s.recv_string(); log.print(buf);
		s.Close();
	}
	hostname=L"";
}

void Smtp::sendmail(
	const char* to,
	const char* subject,
	const char* body,
	const char* from
)
{
	//�f�[�^�𑗎�M
	string buf;
	buf=s.recv_string();
	s.sendf("HELO %ls\r\n",hostname.c_str());	buf=s.recv_string(); log.print(buf);
	s.sendf("MAIL FROM:<%hs>\r\n",from);		buf=s.recv_string(); log.print(buf);
	s.sendf("RCPT TO:<%hs>\r\n",to);			buf=s.recv_string(); log.print(buf);
	s.sendf("DATA\r\n");						buf=s.recv_string(); log.print(buf);
	s.sendf("X-Mailer: DosukoiMailer\r\n");
	s.sendf("Reply-To: %hs\r\n",from);
	s.sendf("Subject: %hs \r\n",subject);
	s.sendf("From: %hs \r\n",from);
	s.sendf("To: %hs \r\n",to);
	s.sendf("\r\n");
	s.sendf("%s\r\n",body);
	s.sendf(".\r\n");							buf=s.recv_string(); log.print(buf);
}

