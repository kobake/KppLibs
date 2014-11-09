#include <cstdio>
#include "wchar.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ”»’è                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           •ÏŠ·                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//•¶Žš¨”’l
int wctoi_radix(int c,int radix)
{
	if(radix>=2 && radix<=10){
		if(c>=L'0' && c<=L'9'+radix-10)return c-L'0';
	}
	else if(radix>=11 && radix<=16){
		if(c>=L'0' && c<=L'9')return c-L'0';
		else if(c>=L'a' && c<=L'f'+radix-16)return c-L'a'+10;
		else if(c>=L'A' && c<=L'F'+radix-16)return c-L'A'+10;
	}
	return -1;
}

//”’l¨•¶Žš
int itowc_radix(int i,int radix)
{
	if(radix>=2 && radix<=10){
		if(i>=0 && i<=radix-1)return L'0'+i;
	}
	else if(radix>=11 && radix<=16){
		if(i>=0 && i<=9)return L'0'+i;
		else if(i>=10 && i<=radix-1)return L'a'+i-10;
	}
	return -1;
}