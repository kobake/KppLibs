#include "CUnitAllocator.h"
#include <windows.h>
#include <assert.h>
#include "CNormalAllocator.h" //TSystemSingleton

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
定数群。

パフォーマンス等の調整。
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
static const size_t	FIELD_SIZE				= 10*1024*1024;									// フィールド本体サイズ (10MB) //

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
フィールド群。

もっともでっかいメモリ塊 (10MB程度)
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
	SField*			m_prev;					// チェーン構造：前 //
	SField*			m_next;					// チェーン構造：次 //
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
		// 接続 //
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
ユニットテーブル。
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
SUnits::SUnits(size_t unitsize)
{
	m_unitsize = unitsize;
	m_head = NULL;
	if(m_unitsize < sizeof(SUnit))m_unitsize = sizeof(SUnit);
}

void SUnits::AttachMemory(void* pData, size_t sizeData)
{
	// -- -- 新しいリンク群をm_headの先頭に挿入する -- -- //
	SUnit* newhead = (SUnit*)pData;
	// 要素数
	size_t sizeCount = sizeData / m_unitsize;
	// 要素間のリンクを張る
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
		// フィールド確保 //
		SField* pNewField = SFields::Instance()->AddNewField();
		// 割り当て
		AttachMemory(pNewField->GetFieldData(), pNewField->GetFieldSize());
	}
	// 先頭ユニットを戻り値とする //
	void* ret = m_head;
	// 先頭を進める //
	if(m_head)m_head = m_head->m_next;
	// 結果 //
	return ret;
}

void SUnits::Free(void* p)
{
	SUnit* pUnit = (SUnit*)p;
	// 先頭に挿入 //
	pUnit->m_next = m_head;
	m_head = pUnit;
}
