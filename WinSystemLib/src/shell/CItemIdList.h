#pragma once

#include <shlobj.h>
#include <string>
#include <boost/shared_ptr.hpp>
class CShellFolder;

//###コピーを高速化したいところ
class CItemIdList{
public:
	//コンストラクタ・デストラクタ
	CItemIdList();
	CItemIdList(const wchar_t* szPath);
	CItemIdList(LPCITEMIDLIST pidl, int nCount = -1);
	CItemIdList(const CItemIdList& rhs);
	CItemIdList& operator = (const CItemIdList& rhs);
	virtual ~CItemIdList();
	void Assign(LPCITEMIDLIST pidl, int nCount = -1);

	//状態
	bool IsValid() const{ return m_pidl != NULL; }

	//所有
	void _Dispose();
	void Attach(LPITEMIDLIST pidl); //所有権を取得する
	LPITEMIDLIST Detach(); //所有権を放棄する

	//操作
	void Clear();

	//インターフェース
	std::wstring GetPath() const;
	std::wstring GetPath(const CShellFolder& cFolder) const;
	int GetCount() const;
	LPCITEMIDLIST GetAt(int nPos) const;

	//開発用
	LPITEMIDLIST  _GetImp()      { return m_pidl; }
	LPCITEMIDLIST _GetImp() const{ return m_pidl; }
	void _Dump() const;

private:
	LPITEMIDLIST	m_pidl;

	std::wstring	m_debug;	//###デバッグ情報を蓄えておく
};

//###仮
#include "CMalloc.h"

class CItemId{
public:
	CItemId(const SHITEMID& sItemId)
	{
		const SHITEMID* pItemId = &sItemId;

		//　アイテムＩＤのサイズを取得します。
		int iSize = pItemId->cb;

		//　新しく領域を確保します。アイテムＩＤはゼロの入った１６ビットサイズの
		//領域で閉じなければならないので、その分だけ多めに確保します。
		m_pItemId = (SHITEMID*)CMalloc().Alloc( iSize + sizeof(USHORT) );

		//　アイテムＩＤをコピーします。
		memcpy( m_pItemId, pItemId, iSize );

		//　ゼロで閉じておきます。
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
