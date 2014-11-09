#include "_required.h"
#include "CDataSocket.h"
#include <string>
#include <vector>
#include <StringLib.h>
using namespace std;

//! 文字列送信
int CDataSocket::sendf(const char* str,...)
{
	//文字列整形
	va_list mark;
	va_start(mark,str);
	const char* str2=tmp_vsprintf(str,mark);
	va_end(mark);

	//送信
	return send(str2,(uint)strlen(str2),0); //###flagsは0とみなす
}

//! ちょっとずつ受信
int CDataSocket::recv_smart(void* buf,uint len,int flags)
{
	int got=0; //受信した合計
	byte* p=(byte*)buf;
	do{
		int ret=this->recv(p,1024,flags);
		if(ret==0){
			//recv が 0 を返したので受信を終了
			break;
		}else if(ret==SOCKET_ERROR){
			//recv が SOCKET_ERROR を返したので受信を終了
			break;
		}
		p+=ret;
		got+=ret;
	}while(got<(int)len);

	return got;
}

//! 文字列を受信できるだけ受信
std::string CDataSocket::recv_string()
{
	vector<char> buf;
	static const int N=1024;

	//Nバイトずつ受信
	while(1){
		//バッファ増やす
		buf.resize(buf.size()+N);

		//増やしたところに受信
		int ret=this->recv(&buf[buf.size()-N],N);

		//戻り値の判定
		if(ret==0){
			//recvが0を返したので受信を終了
			break;
		}
		else if(ret==SOCKET_ERROR){
			//recvがSOCKET_ERRORを返したので受信を終了。###ここでは例外を返したほうがいい？
			break;
		}
		else if(ret>0){
			if(ret<N){
				//受信サイズ補正
				buf.resize(buf.size()-(N-ret));

				//受信終了
				break;
			}
		}
		else{
			//不明な戻り値
			throw wexception(L"recv's return value is invalid");
		}
	}

	//終端に'\0'を付加
	buf.resize(buf.size()+1);
	buf.back()='\0';

	//文字列に変換して返す
	return &buf[0];
}
