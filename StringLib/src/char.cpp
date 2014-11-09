#include <cstdio>
#include "char.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ”»’è                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           •ÏŠ·                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //



//•¶Žš¨”’l
int ctoi_radix(int c,int radix)
{
	if(radix>=2 && radix<=10){
		if(c>='0' && c<='9'+radix-10)return c-'0';
	}
	else if(radix>=11 && radix<=16){
		if(c>='0' && c<='9')return c-'0';
		else if(c>='a' && c<='f'+radix-16)return c-'a'+10;
		else if(c>='A' && c<='F'+radix-16)return c-'A'+10;
	}
	return -1;
}

//”’l¨•¶Žš
int itoc_radix(int i,int radix)
{
	if(radix>=2 && radix<=10){
		if(i>=0 && i<=radix-1)return '0'+i;
	}
	else if(radix>=11 && radix<=16){
		if(i>=0 && i<=9)return '0'+i;
		else if(i>=10 && i<=radix-1)return 'a'+i-10;
	}
	return -1;
}

