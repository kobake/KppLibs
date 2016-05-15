#include "_required.h"
#include "rand.h"

//���`�����@
//UNIX gcc��
//���_�F�ŉ��ʃr�b�g��0��1�̌J��Ԃ��ɂȂ�B�B�B

void UnixRand::init(ulong n)
{
	x=n;
}
ulong UnixRand::next()
{
	x=x*1103515245+12345;
	return x&2147483647;
}



//���`�����@
//VisualC++��

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
