#include "_required.h"
#include "Mail.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  staticインターフェース                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

ushort Mail::getSmtpPort()
{
	ushort port;
	SERVENT *service=getservbyname("mail",NULL);
	if(service==NULL){
		port=IPPORT_SMTP; //ポート指定がされていないので、デフォルトを使う
	}else{
		port=ntohs(service->s_port); //既にhtons済みな値なので、short型ビット配列に戻しておく
	}
	return port;
}

ushort Mail::getPop3Port()
{
	return 110;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

