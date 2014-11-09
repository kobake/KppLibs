#include "CUnitAllocator.h"
#include <windows.h>
#include <assert.h>
#include "CNormalAllocator.h" //TSystemSingleton

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�萔�Q�B

�p�t�H�[�}���X���̒����B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
static const size_t	FIELD_SIZE				= 10*1024*1024;									// �t�B�[���h�{�̃T�C�Y (10MB) //

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�t�B�[���h�Q�B

�����Ƃ��ł������������� (10MB���x)
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
struct SField{
public:
	friend struct SFields;
public:
	void	Init()
	{
		m_prev = NULL;
		m_next = NULL;
		m_mark  = 0xEEEEEEEE;
		m_mark2 = 0xEEEEEEEE;
	}
	void*	GetFieldData() const	{ return m_field; }
	size_t	GetFieldSize() const	{ return sizeof(m_field); }
private:
	SField*			m_prev;					// �`�F�[���\���F�O //
	SField*			m_next;					// �`�F�[���\���F�� //
	size_t			m_mark;					// 0xEEEEEEEE //
	mutable char	m_field[FIELD_SIZE];	// 10MB //
	size_t			m_mark2;				// 0xEEEEEEEE //
};

struct SFields : public TSystemSingleton<SFields>{
public:
	SFields() : m_pHead(NULL), m_pTail(NULL) {}
	~SFields()
	{
		SField* p = m_pHead;
		while(p){
			SField* next = p->m_next;
			::LocalFree(p);
			p = next;
		}
		m_pHead = NULL;
		m_pTail = NULL;
	}
	SField* GetHeadField(){ return m_pHead; }
	SField*	AddNewField()
	{
		SField*	pNew = (SField*)::LocalAlloc(LMEM_FIXED, sizeof(SField));
		pNew->Init();
		// �ڑ� //
		if(!m_pHead){
			m_pHead = pNew;
			m_pTail = pNew;
		}
		else{
			m_pTail->m_next = pNew;
			pNew->m_prev = pNew;
			m_pTail = pNew;
		}
		return pNew;
	}
private:
	SField*	m_pHead;
	SField*	m_pTail;
};




/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
���j�b�g�e�[�u���B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
SUnits::SUnits(size_t unitsize)
{
	m_unitsize = unitsize;
	m_head = NULL;
	if(m_unitsize < sizeof(SUnit))m_unitsize = sizeof(SUnit);
}

void SUnits::AttachMemory(void* pData, size_t sizeData)
{
	// -- -- �V���������N�Q��m_head�̐擪�ɑ}������ -- -- //
	SUnit* newhead = (SUnit*)pData;
	// �v�f��
	size_t sizeCount = sizeData / m_unitsize;
	// �v�f�Ԃ̃����N�𒣂�
	char* p = (char*)pData;
	for(int i = 0; i < (int)sizeCount - 1; i++, p += m_unitsize){
		SUnit* prev = (SUnit*)p;
		SUnit* next = (SUnit*)((char*)prev + m_unitsize);
		prev->m_next = next;
	}
	((SUnit*)p)->m_next = m_head;
	m_head = newhead;
}

void* SUnits::Alloc()
{
	if(!m_head){
		// �t�B�[���h�m�� //
		SField* pNewField = SFields::Instance()->AddNewField();
		// ���蓖��
		AttachMemory(pNewField->GetFieldData(), pNewField->GetFieldSize());
	}
	// �擪���j�b�g��߂�l�Ƃ��� //
	void* ret = m_head;
	// �擪��i�߂� //
	if(m_head)m_head = m_head->m_next;
	// ���� //
	return ret;
}

void SUnits::Free(void* p)
{
	SUnit* pUnit = (SUnit*)p;
	// �擪�ɑ}�� //
	pUnit->m_next = m_head;
	m_head = pUnit;
}
