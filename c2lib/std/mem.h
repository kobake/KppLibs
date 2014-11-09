#pragma once


//!メモリブロック
struct Memory{
	void* data;
	uint length;
	//
	Memory(){}
	Memory(void* _data,uint _length) : data(_data), length(_length) { }
	void clear()
	{
		data=0;
		length=0;
	}
};

Memory mem_alloc(uint size);
void mem_free(Memory mem);


//!読み取り専用メモリブロック
struct ConstMemory{
	const void* data;
	uint length;

	ConstMemory(){}
	ConstMemory(const void* _data,uint _length) : data(_data), length(_length) { }
	ConstMemory(const Memory& rhs) : data(rhs.data), length(rhs.length) { }
	void clear()
	{
		data=0;
		length=0;
	}
};
