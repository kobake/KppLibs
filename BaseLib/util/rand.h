#pragma once

//線形合同法
//UNIX gcc式
//欠点：最下位ビットが0と1の繰り返しになる。。。
class UnixRand{
public:
	UnixRand(ulong n=1){ init(n); }
	void init(ulong n);
	ulong next();
private:
	ulong x;
};

//線形合同法
//VisualC++等
class MajorRand{
private:
	//VisualC++の場合
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

