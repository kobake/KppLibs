#include "CSelectAllocator.h"
#include "CNormalAllocator.h"
#include "CUnitAllocator.h"
#include <assert.h>
#include <windows.h>

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
セット。
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
class CSelectSet : public TSystemSingleton<CSelectSet>{
public:
	CSelectSet() : m_normal(), m_unit32(32), m_unit128(128), m_unit256(256) {}
public:
	CUnitAllocator		m_unit32;
	CUnitAllocator		m_unit128;
	CUnitAllocator		m_unit256;
	CNormalAllocator	m_normal;
};


/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
アロケータ。
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
void* CSelectAllocator::Alloc(size_t size)
{
	size += sizeof(int); // アロケータ情報保存用の領域を作る //
	CSelectSet* s = CSelectSet::Instance();
	char* p = NULL;
	if(size <= 32){
		p = (char*)s->m_unit32.Alloc();
		*((int*)p) = 32;
	}
	else if(size <= 128){
		p = (char*)s->m_unit128.Alloc();
		*((int*)p) = 128;
	}
	else if(size <= 256){
		p = (char*)s->m_unit256.Alloc();
		*((int*)p) = 256;
	}
	else{
//		p = (char*)s->m_normal.Alloc(size);
		p = (char*)::LocalAlloc(LMEM_FIXED, size);
		*((int*)p) = -1;
	}
	p += sizeof(int);
	//###
	if(p == (void*)0x01dc72f9){ //この領域の先頭が壊れることがある
		p = p;
	}
	return p;
}

void CSelectAllocator::Free(void* _p)
{
	if(!_p)return;
	CSelectSet* s = CSelectSet::Instance();
	char* p = (char*)_p - sizeof(int); // 実際の領域を得る //
	//###
	if(p == (void*)0x01dc72f9){ //この領域の先頭が壊れることがある
		p = p;
	}
	switch(*((int*)p)){
	case 32:
		s->m_unit32.Free(p);
		break;
	case 128:
		s->m_unit128.Free(p);
		break;
	case 256:
		s->m_unit256.Free(p);
		break;
	case -1:
		::LocalFree(p);
//		s->m_normal.Free(p);
		break;
	default:
		assert(0);
		break;
	}
}
