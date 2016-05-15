#include "_required.h"
#include "mem.h"
#include <cstdlib>

Memory mem_alloc(uint size)
{
	Memory mem;
	mem.data=malloc(size);
	mem.length=size;
	return mem;
}

void mem_free(Memory mem)
{
	if(mem.data){
		free(mem.data);
	}
}
