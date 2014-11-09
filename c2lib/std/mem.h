#pragma once


//!�������u���b�N
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


//!�ǂݎ���p�������u���b�N
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
