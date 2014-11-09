#pragma once

#include <shlobj.h> //IMalloc
#include <assert.h>

class CMalloc{
public:
	CMalloc()
	{
		HRESULT hr = SHGetMalloc(&m_lpMalloc);
	}
	virtual ~CMalloc()
	{
		m_lpMalloc->Release();
	}

	//インターフェース
	void* Alloc(ULONG size)
	{
		void* p = m_lpMalloc->Alloc(size);
		assert(p!=NULL);
		return p;
	}
	void Free(void* p)
	{
		m_lpMalloc->Free(p);
	}

private:
	IMalloc* m_lpMalloc;
};
