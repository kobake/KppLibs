#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include <MMSystem.h>
#include "f_base.h"

int fmax(int n1,int n2)
{
	if(n1>n2)return n1;
	else return n2;
}

int fmin(int n1,int n2)
{
	if(n1<n2)return n1;
	else return n2;
}

void iswap(int *n1,int *n2)
{
	int tmp=*n1;
	*n1=*n2;
	*n2=tmp;
}

void *cloneMemory(const void *src,size_t size)
{
	void *ret=malloc(size);
	memcpy(ret,src,size);
	return ret;
}

int adjust(int n,int min,int max)
{
	if(n<min)return min;
	if(n>max)return max;
	return n;
}


void nputs(const char *str,int n)
{
	for(int i=0;i<n;i++){
		putchar(str[i]);
	}
}

#include <windows.h>

int doevents()
{
	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
		if(msg.message==WM_QUIT)return -1;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

static unsigned long regulate_timestamp;
void regulate_init()
{
	regulate_timestamp=timeGetTime();
}
int regulate(unsigned long _interval)
{
	unsigned long now=timeGetTime();
	if(now>regulate_timestamp){
		if(now-regulate_timestamp<_interval){
			_interval-=(now-regulate_timestamp);
		}else{
			_interval=0;
		}
	}else{
		_interval=_interval;
	}
	unsigned long nexttime=now+_interval;
	do{
		if(doevents()!=0)return -1;
	}while(timeGetTime()<nexttime);
	regulate_timestamp=timeGetTime();
	return 0;
}




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- 256チャンネル(0～255) ON/OFF 可能 printf -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// チャンネル ON フラグ
static int on_ch[256];

// 初期状態設定
static void _ch_printf_init_once()
{
	static int init=0;
	if(!init){
		init=1;
		for(int i=0;i<256;i++){
			on_ch[i]=1;
		}
	}
}

// 256チャンネル(0～255) printf
int ch_printf(int ch,const char *format,...)
{
	_ch_printf_init_once();
	//
	if(on_ch[ch]){
		va_list mark;
		va_start(mark,format);
		int ret=vprintf(format,mark);
		va_end(mark);
		return ret;
	}else{
		return 0;
	}
}

// ON/OFF
void ch_printf_on(int ch)
{
	_ch_printf_init_once();
	on_ch[ch]=1;
}
void ch_printf_off(int ch)
{
	_ch_printf_init_once();
	on_ch[ch]=0;
}
