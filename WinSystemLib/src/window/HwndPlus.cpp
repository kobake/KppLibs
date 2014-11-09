#include "_required.h"
#include "HwndPlus.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  ���Ȃ�HWND�ɋ߂��N���X                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

HwndPlus::HwndPlus(HWND _hwnd)
: hwnd(_hwnd)
{
}

void HwndPlus::writeWindowData(const void* _data,size_t size)
{
	const byte* data=(byte*)_data;
	const byte* p=data;
	LONG n=0;
	for(int i=0;i<(int)size;i++){
		if(i%4==0)n=0; //�J�n
		n|=( (*p++) << 8*(i%4) );
		if(i%4==3 || i==size-1)SetWindowLong(hwnd,i/4*4,n); //�I��
	}
}

void HwndPlus::readWindowData(void* _data,size_t size)
{
	byte* data=(byte*)_data;
	byte* p=data;
	LONG n=0;
	for(int i=0;i<(int)size;i++){
		if(i%4==0)n=GetWindowLong(hwnd,i); //�J�n
		*p++=(byte)(n&0xFF);
		n>>=8;
	}
}

void HwndPlus::setWindowPointer(const void* p)
{
	writeWindowData(&p,sizeof(void*));
	//SetWindowLong(hwnd,GWL_USERDATA,(LONG)p);
}

void* HwndPlus::getWindowPointer()
{
	void* p;
	readWindowData(&p,sizeof(void*));
	//p=(void*)GetWindowLong(hwnd,GWL_USERDATA);
	return p;
}
