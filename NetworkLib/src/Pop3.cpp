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
//               コンストラクタ・デストラクタ                  //
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
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! サーバに接続
void Pop3::Connect(const CIpAddress& ip)
{
	string buf;

	//接続
	CSocketAddress addr(ip,Mail::getPop3Port());
	s.Connect(addr);

	//初めのメッセージを取得
	buf=s.recv_string(); log.print(buf);
}

//! ログイン
void Pop3::login(const wchar_t* user,const wchar_t* pass)
{
	string buf;

	//ログイン
	s.sendf("USER %ls\r\n",user); buf=s.recv_string(); log.print(buf);
	s.sendf("PASS %ls\r\n",pass); buf=s.recv_string(); log.print(buf);

	//サーバからのメッセージを解釈
	if(buf.find("0 message")!=string::npos){
		//メッセージなし。接続を切る。
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

	//簡易情報を取得 (LISTコマンド)
	s.sendf("LIST %d\r\n",no); buf=s.recv_string(); log.print(buf);

	//メールが存在しない場合は例外
	if(buf.compare(0,4,"-ERR")==0)throw myexception(L"mail[%d] is not found",no);

	//メールサイズを取得
	int mailsize=0;
	{
		char *token;
		char seps[] = " ";
	    
		//半角スペースをデリミタとして３番目のトークンを取得する
		//「+OK 番号 **** octes」という文字列から****を取得する
		token = strtok(&buf[0], seps);//無駄読み１番目のトークン
		token = strtok(NULL, seps);//無駄読み２番目のトークン
		token = strtok(NULL, seps);//３番目のトークン
		mailsize=atoi(token);
	}

	//読み出す (RETRコマンド)
	vector<char> mail; mail.resize(mailsize+1);
	s.sendf("RETR %d\r\n",no);
	int gotsize=s.recv_smart(&mail[0],mailsize);
	mail[gotsize]='\0';
	assert(gotsize==mailsize);

	//###JIS→SJISに変換する必要がある？

	return mystring(&mail[0]);
}

