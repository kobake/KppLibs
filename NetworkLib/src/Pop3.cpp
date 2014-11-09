#include "_required.h"
#include "Pop3.h"
#include "CSocket.h"
#include "Mail.h"
#include "CSocketAddress.h"
#include "CIpAddress.h"
#include "std/myexception.h"
#include <vector>
#include <DebugLib.h>
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Pop3::Pop3(bool _log_print)
: nmessage(0)
, s(&s2)
{
	log.setEnabled(_log_print);
}

Pop3::~Pop3()
{
	quit();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �C���^�[�t�F�[�X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! �T�[�o�ɐڑ�
void Pop3::Connect(const CIpAddress& ip)
{
	string buf;

	//�ڑ�
	CSocketAddress addr(ip,Mail::getPop3Port());
	s.Connect(addr);

	//���߂̃��b�Z�[�W���擾
	buf=s.recv_string(); log.print(buf);
}

//! ���O�C��
void Pop3::login(const wchar_t* user,const wchar_t* pass)
{
	string buf;

	//���O�C��
	s.sendf("USER %ls\r\n",user); buf=s.recv_string(); log.print(buf);
	s.sendf("PASS %ls\r\n",pass); buf=s.recv_string(); log.print(buf);

	//�T�[�o����̃��b�Z�[�W������
	if(buf.find("0 message")!=string::npos){
		//���b�Z�[�W�Ȃ��B�ڑ���؂�B
		quit();
	}
}

void Pop3::quit()
{
	if(s.valid()){
		char buf[1024];
		s.sendf("QUIT\r\n"); s.recv(buf,sizeof(buf)-1);
		s.Close();
	}
}

Pop3::mystring Pop3::getMail(int no)
{
	mystring buf;

	//�ȈՏ����擾 (LIST�R�}���h)
	s.sendf("LIST %d\r\n",no); buf=s.recv_string(); log.print(buf);

	//���[�������݂��Ȃ��ꍇ�͗�O
	if(buf.compare(0,4,"-ERR")==0)throw myexception(L"mail[%d] is not found",no);

	//���[���T�C�Y���擾
	int mailsize=0;
	{
		char *token;
		char seps[] = " ";
	    
		//���p�X�y�[�X���f���~�^�Ƃ��ĂR�Ԗڂ̃g�[�N�����擾����
		//�u+OK �ԍ� **** octes�v�Ƃ��������񂩂�****���擾����
		token = strtok(&buf[0], seps);//���ʓǂ݂P�Ԗڂ̃g�[�N��
		token = strtok(NULL, seps);//���ʓǂ݂Q�Ԗڂ̃g�[�N��
		token = strtok(NULL, seps);//�R�Ԗڂ̃g�[�N��
		mailsize=atoi(token);
	}

	//�ǂݏo�� (RETR�R�}���h)
	vector<char> mail; mail.resize(mailsize+1);
	s.sendf("RETR %d\r\n",no);
	int gotsize=s.recv_smart(&mail[0],mailsize);
	mail[gotsize]='\0';
	assert(gotsize==mailsize);

	//###JIS��SJIS�ɕϊ�����K�v������H

	return mystring(&mail[0]);
}

