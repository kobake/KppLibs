#pragma once

class CSelectAllocator{
public:
	static void*	Alloc(size_t size);
	static void		Free(void* p);
};
