#pragma once

//���`�����@
//UNIX gcc��
//���_�F�ŉ��ʃr�b�g��0��1�̌J��Ԃ��ɂȂ�B�B�B
class UnixRand{
public:
	UnixRand(ulong n=1){ init(n); }
	void init(ulong n);
	ulong next();
private:
	ulong x;
};

//���`�����@
//VisualC++��
class MajorRand{
private:
	//VisualC++�̏ꍇ
	static const ulong A=214013;
	static const ulong C=2531011;
	static const ulong F=0;
	static const ulong S=1;
public:
	MajorRand(ulong n=S){ init(n); }
	void init(ulong n);
	ulong next();
public:
	static const ulong MIN=0;
	static const ulong MAX=32767; //0x7FFF
private:
	ulong x;
};

