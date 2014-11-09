#pragma once

#include <windows.h>
#include <objidl.h>

class CStream : public IStream{
public:
	CStream();
	~CStream();

	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	STDMETHODIMP Read(void* pv, ULONG cb, ULONG* pcbRead);
	STDMETHODIMP Write(void const* pv, ULONG cb, ULONG* pcbWritten );
	STDMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition);
	STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize );
	STDMETHODIMP CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);
	STDMETHODIMP Commit(DWORD grfCommitFlags);
	STDMETHODIMP Revert(void);
	STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	STDMETHODIMP Stat(STATSTG* pstatstg, DWORD grfStatFlag);
	STDMETHODIMP Clone(IStream** ppstm);

};
