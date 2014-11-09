#pragma once

#include <shlobj.h>
#include <string>
#include <boost/shared_ptr.hpp>
class CShellFolder;

//###�R�s�[���������������Ƃ���
class CItemIdList{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CItemIdList();
	CItemIdList(const wchar_t* szPath);
	CItemIdList(LPCITEMIDLIST pidl, int nCount = -1);
	CItemIdList(const CItemIdList& rhs);
	CItemIdList& operator = (const CItemIdList& rhs);
	virtual ~CItemIdList();
	void Assign(LPCITEMIDLIST pidl, int nCount = -1);

	//���
	bool IsValid() const{ return m_pidl != NULL; }

	//���L
	void _Dispose();
	void Attach(LPITEMIDLIST pidl); //���L�����擾����
	LPITEMIDLIST Detach(); //���L�����������

	//����
	void Clear();

	//�C���^�[�t�F�[�X
	std::wstring GetPath() const;
	std::wstring GetPath(const CShellFolder& cFolder) const;
	int GetCount() const;
	LPCITEMIDLIST GetAt(int nPos) const;

	//�J���p
	LPITEMIDLIST  _GetImp()      { return m_pidl; }
	LPCITEMIDLIST _GetImp() const{ return m_pidl; }
	void _Dump() const;

private:
	LPITEMIDLIST	m_pidl;

	std::wstring	m_debug;	//###�f�o�b�O����~���Ă���
};

//###��
#include "CMalloc.h"

class CItemId{
public:
	CItemId(const SHITEMID& sItemId)
	{
		const SHITEMID* pItemId = &sItemId;

		//�@�A�C�e���h�c�̃T�C�Y���擾���܂��B
		int iSize = pItemId->cb;

		//�@�V�����̈���m�ۂ��܂��B�A�C�e���h�c�̓[���̓������P�U�r�b�g�T�C�Y��
		//�̈�ŕ��Ȃ���΂Ȃ�Ȃ��̂ŁA���̕��������߂Ɋm�ۂ��܂��B
		m_pItemId = (SHITEMID*)CMalloc().Alloc( iSize + sizeof(USHORT) );

		//�@�A�C�e���h�c���R�s�[���܂��B
		memcpy( m_pItemId, pItemId, iSize );

		//�@�[���ŕ��Ă����܂��B
		memset( (LPBYTE)m_pItemId + iSize, 0, sizeof(USHORT) );
	}
	~CItemId()
	{
		CMalloc().Free(m_pItemId);
	}
	const ITEMIDLIST* _GetItemIdList() const
	{
		return (const ITEMIDLIST*)m_pItemId;
	}
private:
	SHITEMID* m_pItemId;
};
/*
LPITEMIDLIST CopyItemID(const ITEMIDLIST* p_pItemIDList) 
{ 




	return pNewIDList;
}
*/
