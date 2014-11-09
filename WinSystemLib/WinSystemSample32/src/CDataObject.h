/*
	éQçlÅFhttp://eternalwindows.jp/ole/oledata/oledata05.html
*/
#pragma once

#include <windows.h>
#include <objidl.h>

class CDataObject : public IDataObject, public IEnumFORMATETC{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	
	STDMETHODIMP GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
	STDMETHODIMP GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium);
	STDMETHODIMP QueryGetData(FORMATETC *pformatetc);
	STDMETHODIMP GetCanonicalFormatEtc(FORMATETC *pformatectIn, FORMATETC *pformatetcOut);
	STDMETHODIMP SetData(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease);
	STDMETHODIMP EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc);
	STDMETHODIMP DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
	STDMETHODIMP DUnadvise(DWORD dwConnection);
	STDMETHODIMP EnumDAdvise(IEnumSTATDATA **ppenumAdvise);
	
	STDMETHODIMP Next(ULONG celt, FORMATETC *rgelt, ULONG *pceltFetched);
	STDMETHODIMP Skip(ULONG celt);
	STDMETHODIMP Reset(VOID);
	STDMETHODIMP Clone(IEnumFORMATETC **ppenum);
	
	CDataObject();
	~CDataObject();

private:
	LONG  m_cRef;
	ULONG m_uEnumCount;
	char  m_szText[256];
};
