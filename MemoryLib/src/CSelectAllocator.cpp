#include "CSelectAllocator.h"
#include "CNormalAllocator.h"
#include "CUnitAllocator.h"
#include <assert.h>
#include <windows.h>

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�Z�b�g�B
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
�A���P�[�^�B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
void* CSelectAllocator::Alloc(size_t size)
{
	size += sizeof(int); // �A���P�[�^���ۑ��p�̗̈����� //
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
	if(p == (void*)0x01dc72f9){ //���̗̈�̐擪�����邱�Ƃ�����
		p = p;
	}
	return p;
}

void CSelectAllocator::Free(void* _p)
{
	if(!_p)return;
	CSelectSet* s = CSelectSet::Instance();
	char* p = (char*)_p - sizeof(int); // ���ۂ̗̈�𓾂� //
	//###
	if(p == (void*)0x01dc72f9){ //���̗̈�̐擪�����邱�Ƃ�����
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
