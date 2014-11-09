#include "_required.h"
#include "CDataSocket.h"
#include <string>
#include <vector>
#include <StringLib.h>
using namespace std;

//! �����񑗐M
int CDataSocket::sendf(const char* str,...)
{
	//�����񐮌`
	va_list mark;
	va_start(mark,str);
	const char* str2=tmp_vsprintf(str,mark);
	va_end(mark);

	//���M
	return send(str2,(uint)strlen(str2),0); //###flags��0�Ƃ݂Ȃ�
}

//! ������Ƃ���M
int CDataSocket::recv_smart(void* buf,uint len,int flags)
{
	int got=0; //��M�������v
	byte* p=(byte*)buf;
	do{
		int ret=this->recv(p,1024,flags);
		if(ret==0){
			//recv �� 0 ��Ԃ����̂Ŏ�M���I��
			break;
		}else if(ret==SOCKET_ERROR){
			//recv �� SOCKET_ERROR ��Ԃ����̂Ŏ�M���I��
			break;
		}
		p+=ret;
		got+=ret;
	}while(got<(int)len);

	return got;
}

//! ���������M�ł��邾����M
std::string CDataSocket::recv_string()
{
	vector<char> buf;
	static const int N=1024;

	//N�o�C�g����M
	while(1){
		//�o�b�t�@���₷
		buf.resize(buf.size()+N);

		//���₵���Ƃ���Ɏ�M
		int ret=this->recv(&buf[buf.size()-N],N);

		//�߂�l�̔���
		if(ret==0){
			//recv��0��Ԃ����̂Ŏ�M���I��
			break;
		}
		else if(ret==SOCKET_ERROR){
			//recv��SOCKET_ERROR��Ԃ����̂Ŏ�M���I���B###�����ł͗�O��Ԃ����ق��������H
			break;
		}
		else if(ret>0){
			if(ret<N){
				//��M�T�C�Y�␳
				buf.resize(buf.size()-(N-ret));

				//��M�I��
				break;
			}
		}
		else{
			//�s���Ȗ߂�l
			throw wexception(L"recv's return value is invalid");
		}
	}

	//�I�[��'\0'��t��
	buf.resize(buf.size()+1);
	buf.back()='\0';

	//������ɕϊ����ĕԂ�
	return &buf[0];
}
