#include "_required.h"
#include "rand.h"

//線形合同法
//UNIX gcc式
//欠点：最下位ビットが0と1の繰り返しになる。。。

void UnixRand::init(ulong n)
{
	x=n;
}
ulong UnixRand::next()
{
	x=x*1103515245+12345;
	return x&2147483647;
}



//線形合同法
//VisualC++等

void MajorRand::init(ulong n)
{
	x=n;
	if(F)next();
}

ulong MajorRand::next()
{
	x=x*A+C;
	return (ulong)(x>>16)&32767;
}
