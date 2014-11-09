#include "CNormalAllocator.h"
#include <windows.h>
#include <assert.h>

struct SField;
struct SFreeSpace;

//#####
bool g_debug = false;
int g_count = 0;

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�萔�Q�B

�p�t�H�[�}���X���̒����B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
static const size_t	FIELD_SIZE				= 200;//###10*1024*1024;									// �t�B�[���h�{�̃T�C�Y (10MB) //
static const size_t	FREESPACE_STATUS_SIZE	= 24;											// �t���[�X�y�[�X�����T�C�Y //
static const size_t	FREESPACE_MAX_SIZE		= FIELD_SIZE - FREESPACE_STATUS_SIZE;			// �t���[�X�y�[�X�{�̍ő�T�C�Y //
static const size_t BLOCK_STATUS_SIZE		= sizeof(size_t) + sizeof(size_t);				// �u���b�N�����T�C�Y //
static const size_t BLOCK_MAX_SIZE			= FREESPACE_MAX_SIZE - BLOCK_STATUS_SIZE;		// �u���b�N�{�̍ő�T�C�Y //
static const size_t BLOCK_MIN_SIZE			= FREESPACE_STATUS_SIZE - BLOCK_STATUS_SIZE;	// �u���b�N�{�̍ŏ��T�C�Y (�t���[�X�y�[�X�̍ŏ��T�C�Y�ɍ��킹��) //



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
		m_mark2 = 0x99999999;
		//###test
		memset(m_field, 0x77777777, sizeof(m_field));
	}
	void*	GetFieldData() const	{ return m_field; }
	size_t	GetFieldSize() const	{ return sizeof(m_field); }
private:
	SField*			m_prev;					// �`�F�[���\���F�O //
	SField*			m_next;					// �`�F�[���\���F�� //
	size_t			m_mark;					// 0xEEEEEEEE //
	mutable char	m_field[FIELD_SIZE];	// 10MB //
	size_t			m_mark2;				// 0x99999999 //
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
�g�p�ς݃u���b�N�B

1���Alloc�ł��̍\�����ЂƂm�ۂ����B
[size][body][AAAAAAAA]
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
struct SBlock{
public:
	void	InitBlock(size_t requiredsize);
	// �����擾 //
	size_t	GetAllSize() const	{ return sizeof(size_t) + m_datasize + sizeof(size_t); }
	size_t	GetDataSize() const	{ return m_datasize; }
	void*	GetData() const		{ return m_block_and_tail; }
	// ���� //
	void	Free();
public:
	static SBlock* S_Attach(void* p)
	{
		return (SBlock*)((char*)p - sizeof(size_t));
	}
private:
	size_t			m_datasize;				// �f�[�^�T�C�Y //
	mutable char	m_block_and_tail[1];	// ���ۂ̃T�C�Y�� m_datasize + sizeof(size_t)�B�Ō��sizeof(size_t)�ɂ�AAAAAAAA���i�[����� //
};

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�t���[�X�y�[�X�B

�t�B�[���h���̃t���[�̈�B
[FFFFFFFF][prev][next][size][spacebody][size][DDDDDDDD]
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
struct SFreeSpace{
public:
	friend struct SFreeSpaces;
public:
	void	Init(size_t fieldsize)
	{
		size_t spacesize = fieldsize - sizeof(m_headmark) - sizeof(m_prev) - sizeof(m_next) - sizeof(m_spacesize) - sizeof(size_t) - sizeof(size_t);

		m_headmark = 0xFFFFFFFF;
		m_prev = NULL;
		m_next = NULL;
		m_spacesize = spacesize;
		_CopySizeToTail();
	}
	void*		Alloc(size_t requiredsize);
	// ���擾 //
	size_t		GetSpaceSize() const	{ return m_spacesize; }
	size_t		GetAllSize() const		{ return sizeof(m_headmark) + sizeof(m_prev) + sizeof(m_next) + sizeof(m_spacesize) + m_spacesize + sizeof(size_t) + sizeof(size_t); }
	const void*	_GetEndPointer() const	{ return (char*)this + GetAllSize(); }
	// �g�� //
	void		MergeTo(SFreeSpace* pMergeSpace);
	void		Extend(size_t extendsize)
	{
		m_spacesize += extendsize;
		_CopySizeToTail();
	}
	void	ExtendToPrev(size_t extendsize);
	// �����⏕ //
	void	_CopySizeToTail();
	// �e�X�g�p�@�\ //
	void _CheckCollision(const void* pData, size_t sizeData)
	{
		const void* pStart = pData;
		const void* pEnd = (char*)pData + sizeData;
		const void* qStart = this;
		const void* qEnd = (char*)this + GetAllSize();
		if((pStart < qEnd && pEnd > qStart) || (pStart > qEnd && pEnd < qStart)){
			// �Փ˂��� //
			assert(0);
		}
	}

public:
	static SFreeSpace*	S_AttachFromTail(size_t* tail)
	{
		size_t* tailsize = tail - 1;
		char* p = (char*)tailsize - *tailsize - sizeof(size_t) - sizeof(SFreeSpace*) - sizeof(SFreeSpace*) - sizeof(size_t);
		if(p && ((SFreeSpace*)p)->m_next==(void*)0xFFFFFFFF)assert(0); //###
		return (SFreeSpace*)p;
	}
private:
	size_t		m_headmark;				// 0xFFFFFFFF //
	SFreeSpace*	m_prev;					// �`�F�[���\���F�O //
	SFreeSpace*	m_next;					// �`�F�[���\���F�� //
	size_t		m_spacesize;			// �X�y�[�X�T�C�Y //
	char		m_space_and_tail[1];	// ���ۂ̃T�C�Y�� m_spacesize + sizeof(size_t)(m_spacesize�Ɠ����l) + sizeof(size_t)(0xDDDDDDDD)�B //
};

struct SFreeSpaces : public TSystemSingleton<SFreeSpaces>{
public:
	friend struct SFreeSpace;
public:
	SFreeSpaces() : m_pHead(NULL), m_pTail(NULL) {}
	SFreeSpace*	FindFreeSpace(size_t requiredsize)
	{
		size_t blocksize = requiredsize + sizeof(size_t) + sizeof(size_t);

		SFreeSpace* p = m_pHead;
		while(p){
			if(p->GetSpaceSize() >= blocksize){
				return p;
			}
			p = p->m_next;
		}
		if(p && p->m_next==(void*)0xFFFFFFFF)assert(0); //###
		return p;
	}
	SFreeSpace*	AddNewFreeSpace(SField* pNewField)
	{
		return AddNewFreeSpace(pNewField->GetFieldData(), pNewField->GetFieldSize());
	}
	SFreeSpace*	AddNewFreeSpace(void* pStart, size_t allsize)
	{
		SFreeSpace*	pNew = (SFreeSpace*)pStart;
		pNew->Init(allsize);
		// �ڑ� //
		if(!m_pHead){
			m_pHead = pNew;
			m_pTail = pNew;
		}
		else{
			m_pTail->m_next = pNew;
			pNew->m_prev = m_pTail;
			m_pTail = pNew;
		}
		if(pNew && pNew->m_next==(void*)0xFFFFFFFF)assert(0); //###
		return pNew;
	}
	// �`�F�[������ //
	void		ChainRemove(SFreeSpace* p)
	{
		//###
		int test = _GetAllCount();

		if(p && p->m_next==(void*)0xFFFFFFFF)assert(0); //###
		if(p->m_prev)p->m_prev->m_next = p->m_next;
		if(p->m_next)p->m_next->m_prev = p->m_prev;
		if(p == m_pHead)m_pHead = p->m_next;
		if(p == m_pTail)m_pTail = p->m_prev;

		//###
		int test2 = _GetAllCount();
		assert(test2 == test - 1);
		_CheckAll();
	}
	// �`�F�b�N�@�\ //
	void _CheckCollision(const void* pData, size_t sizeData, SFreeSpace* pIgnore = NULL)
	{
		SFreeSpace* p;
		for(p = m_pHead; p; p = p->m_next){
			if(pIgnore && pIgnore == p)continue;
			p->_CheckCollision(pData, sizeData);
		}
	}
	void _CheckAll()
	{
		SFreeSpace* p = m_pHead;
		while(p){
			if(p->m_headmark != 0xFFFFFFFF){
				assert(0);
			}
			p = p->m_next;
		}
	}
	int	_GetAllCount() const
	{
		int n = 0;
		SFreeSpace* p = m_pHead;
		while(p){
			p = p->m_next;
			n++;
		}
		return n;
	}
private:
	SFreeSpace*	m_pHead;	// �`�F�[���\���F�擪 //	// ���{���̓X�y�[�X�̃T�C�Y���Ƀ\�[�g�\�肾���ǁA���̂Ƃ���̓\�[�g���� //
	SFreeSpace*	m_pTail;	// �`�F�[���\���F���� //
};

void SFreeSpace::_CopySizeToTail()
{
	size_t* tailsize = (size_t*)(&m_space_and_tail[m_spacesize]);
	tailsize[0] = m_spacesize;
	tailsize[1] = 0xDDDDDDDD;
	//###test
	memset(m_space_and_tail, 0x66666666, m_spacesize);
	SFreeSpaces::Instance()->_CheckCollision(m_space_and_tail, m_spacesize, this);
}

void SBlock::InitBlock(size_t requiredsize)
{
	//##################���̃u���b�N���������Ƃ��ɖ�肪�N����
	if(this == (void*)0x00154ed0 && requiredsize == 0x44){
		requiredsize = requiredsize;
		g_debug = true;
	}
	//###test�F�����X�y�[�X�Ɣ���Ă��Ȃ����Ƃ��m�F
	SFreeSpaces::Instance()->_CheckCollision(this, this->GetAllSize());

	m_datasize = requiredsize;
	size_t* tail = (size_t*)(&m_block_and_tail[requiredsize]);
	*tail = 0xAAAAAAAA;
	//###test
	memset(m_block_and_tail, 0x55555555, GetDataSize());
}

void SFreeSpace::MergeTo(SFreeSpace* pMergeSpace)
{
	assert((void*)this < (void*)pMergeSpace);
	size_t extendsize = (char*)pMergeSpace->_GetEndPointer() - (char*)this->_GetEndPointer();
	
	//###test
	if(extendsize > FIELD_SIZE){
		assert(0);
	}
	const void* test = this->_GetEndPointer();

	SFreeSpaces::Instance()->ChainRemove(pMergeSpace);
	Extend(extendsize);
}

void SFreeSpace::ExtendToPrev(size_t extendsize)
{
	//###�������Ȃ��ł݂�i��������������Ȃ����ۂ��j
	return;
	m_spacesize += extendsize;
	SFreeSpace* pNewThis = (SFreeSpace*)(((char*)this) - extendsize);
	if(this->m_prev)this->m_prev->m_next = pNewThis;
	if(this->m_next)this->m_next->m_prev = pNewThis;
	if(SFreeSpaces::Instance()->m_pHead == this)SFreeSpaces::Instance()->m_pHead = pNewThis;
	if(SFreeSpaces::Instance()->m_pTail == this)SFreeSpaces::Instance()->m_pTail = pNewThis;
	pNewThis->m_headmark = m_headmark;
	pNewThis->m_prev = m_prev;
	pNewThis->m_next = m_next;
	pNewThis->m_spacesize = m_spacesize;
	pNewThis->_CopySizeToTail();
	if(this && this->m_next==(void*)0xFFFFFFFF)assert(0); //###
}

void* SFreeSpace::Alloc(size_t requiredsize)
{
	size_t blocksize = requiredsize + sizeof(size_t) + sizeof(size_t);
	assert(blocksize <= m_spacesize);

	// this���̂���SBlock�Ƃ��� //
	SBlock*	pBlock = (SBlock*)this;

	// ���������炷 //
	m_spacesize -= blocksize;
	if(m_spacesize > 0){
		SFreeSpace* pNewThis = (SFreeSpace*)(((char*)this) + blocksize);
		if(this->m_prev)this->m_prev->m_next = pNewThis;
		if(this->m_next)this->m_next->m_prev = pNewThis;
		if(SFreeSpaces::Instance()->m_pHead == this)SFreeSpaces::Instance()->m_pHead = pNewThis;
		if(SFreeSpaces::Instance()->m_pTail == this)SFreeSpaces::Instance()->m_pTail = pNewThis;
		pNewThis->m_spacesize = m_spacesize;
		pNewThis->m_next = m_next;
		pNewThis->m_prev = m_prev;
		pNewThis->m_headmark = m_headmark;
		pNewThis->_CopySizeToTail();
	}
	else{
		// �������͍�� //
		SFreeSpaces::Instance()->ChainRemove(this);
	}

	// SBlock�̏����� //
	pBlock->InitBlock(requiredsize);
	if(this && this->m_next==(void*)0xFFFFFFFF)assert(0); //###

	// ���� //
	return pBlock->GetData();
}

void SBlock::Free()
{
	//##################���̃u���b�N���������Ƃ��ɖ�肪�N����
	if(this == (void*)0x00154ed0 && this->m_datasize == 0x44){
		g_debug = g_debug;
	}

	// prev���t���[�X�y�[�X���ǂ��� -> pPrevFreeSpace //
	SFreeSpace* pPrevFreeSpace = NULL;
	size_t* prev = (size_t*)(((char*)this) - sizeof(size_t));
	if(*prev == 0xDDDDDDDD){
		pPrevFreeSpace = SFreeSpace::S_AttachFromTail(prev);
	}

	// next���t���[�X�y�[�X���ǂ��� -> pNextFreeSpace //
	SFreeSpace* pNextFreeSpace = NULL;
	size_t* next = (size_t*)(m_block_and_tail + m_datasize + sizeof(size_t));
	if(*next == 0xFFFFFFFF){
		pNextFreeSpace = (SFreeSpace*)next;
	}

	// �������t���[�X�y�[�X�Ȃ�A�t���[�X�y�[�X�𓝍� //
	if(pPrevFreeSpace && pNextFreeSpace){
		//###################�������Ȃ�
		if(pPrevFreeSpace == (void*)0x00154e7c && pNextFreeSpace == (void*)0x00154f16){
			pPrevFreeSpace = pPrevFreeSpace;
		}
		pPrevFreeSpace->MergeTo(pNextFreeSpace);
	}
	// prev�̂݃t���[�X�y�[�X�Ȃ�Aprev�t���[�X�y�[�X���g�� //
	else if(pPrevFreeSpace){
		pPrevFreeSpace->Extend(GetAllSize());
	}
	// next�̂݃t���[�X�y�[�X�Ȃ�Anext�t���[�X�y�[�X���g�� //
	else if(pNextFreeSpace){
		pNextFreeSpace->ExtendToPrev(GetAllSize());
	}
	// �ǂ������t���[�X�y�[�X����Ȃ�������A�V�����t���[�X�y�[�X���쐬 //
	else{
		SFreeSpaces::Instance()->AddNewFreeSpace(this, GetAllSize());
	}
}

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�A���P�[�^�B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
void* CNormalAllocator::Alloc(size_t size)
{
	assert(size <= BLOCK_MAX_SIZE);
	// �Œ�T�C�Y�܂Ő؂�グ�� //
	if(size < BLOCK_MIN_SIZE)size = BLOCK_MIN_SIZE;

	// 4�o�C�g�p�f�B���O //
	size = (size + 3) / 4 * 4;

	// �X�y�[�X��T�� //
	SFreeSpace* pSpace = SFreeSpaces::Instance()->FindFreeSpace(size);

	// �X�y�[�X��������Ȃ�������A�V�����t�B�[���h���m�ۂ��� //
	if(!pSpace){
		SField* pNewField = SFields::Instance()->AddNewField();
		if(!pNewField){
			assert(0);
		}
		pSpace = SFreeSpaces::Instance()->AddNewFreeSpace(pNewField);
	}

	// �X�y�[�X����u���b�N���m�� //
	assert(pSpace);
	return pSpace->Alloc(size);
}

void CNormalAllocator::Free(void* p)
{
	SBlock* pBlock = SBlock::S_Attach(p);
	pBlock->Free();
}
