#pragma once

struct SUnits;

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
���j�b�g�e�[�u���B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
struct SUnit{
public:
	friend struct SUnits;
private:
	SUnit*	m_next;
};

struct SUnits{
public:
	friend struct SUnit;
public:
	SUnits(size_t unitsize);
	void	AttachMemory(void* pData, size_t sizeData);
	void*	Alloc();
	void	Free(void* p);
private:
	size_t	m_unitsize;
	SUnit*	m_head;
};

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�A���P�[�^�B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
class CUnitAllocator{
public:
	CUnitAllocator(size_t unitsize) : m_units(unitsize) {}
	void*	Alloc()
	{
		return m_units.Alloc();
	}
	void	Free(void* p)
	{
		m_units.Free(p);
	}
private:
	SUnits	m_units;
};
