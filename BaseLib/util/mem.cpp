#include "_required.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

//ƒƒ‚ƒŠ•¡»
void* memdup(const void* src, int size)
{
	void* ret=malloc(size);
	memcpy(ret, src, size);
	return ret;
}

bool is_mem_zero(const void* _p, int size)
{
	const char* p = (const char*)_p;
	for(int i=0;i<size;i++){
		if(*p++!=0)return false;
	}
	return true;
}
