#include "_required.h"
#include "Mail.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  static�C���^�[�t�F�[�X                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

ushort Mail::getSmtpPort()
{
	ushort port;
	SERVENT *service=getservbyname("mail",NULL);
	if(service==NULL){
		port=IPPORT_SMTP; //�|�[�g�w�肪����Ă��Ȃ��̂ŁA�f�t�H���g���g��
	}else{
		port=ntohs(service->s_port); //����htons�ς݂Ȓl�Ȃ̂ŁAshort�^�r�b�g�z��ɖ߂��Ă���
	}
	return port;
}

ushort Mail::getPop3Port()
{
	return 110;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

