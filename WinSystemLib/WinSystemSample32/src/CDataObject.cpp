#include "CDataObject.h"
#include <WinSystemLib.h>
#include <shlobj.h> //DROPFILES

extern HWND g_hwndMain;
int g_paste_count = 0;
int g_test = 0;

// Win7用のwindows.hにしか入ってないので、独自定義。//
#if 0
template<typename T> void** IID_PPV_ARGS_Helper(T** pp)
{
    // make sure everyone derives from IUnknown
    static_cast<IUnknown*>(*pp);
    return reinterpret_cast<void**>(pp);
}
#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)
#endif

CDataObject::CDataObject()
{
	m_cRef = 1;
	m_uEnumCount = 0;
	lstrcpyA(m_szText, "sample");
}

CDataObject::~CDataObject()
{
//	::MessageBox(NULL, L"~CDataObject", L"Info", MB_OK);
}

STDMETHODIMP CDataObject::QueryInterface(REFIID riid, void **ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDataObject))
		*ppvObject = static_cast<IDataObject *>(this);
	else if (IsEqualIID(riid, IID_IEnumFORMATETC))
		*ppvObject = static_cast<IEnumFORMATETC *>(this);
	else
		return E_NOINTERFACE;

	AddRef();
	
	return S_OK;
}

STDMETHODIMP_(ULONG) CDataObject::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CDataObject::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		delete this;
		return 0;
	}

	return m_cRef;
}

/*
typedef struct tagFORMATETC 
{ 
    CLIPFORMAT      cfFormat; 
    DVTARGETDEVICE  *ptd; 
    DWORD           dwAspect; 
    LONG            lindex; 
    DWORD           tymed; 
}
FORMATETC, *LPFORMATETC;
*/
/*
typedef struct tagSTGMEDIUM 
{ 
    DWORD tymed; 
    [switch_type(DWORD), switch_is((DWORD) tymed)] 
    union { 
        [case(TYMED_GDI)]      HBITMAP        hBitmap; 
        [case(TYMED_MFPICT)]   HMETAFILEPICT  hMetaFilePict; 
        [case(TYMED_ENHMF)]    HENHMETAFILE   hEnhMetaFile; 
        [case(TYMED_HGLOBAL)]  HGLOBAL        hGlobal; 
        [case(TYMED_FILE)]     LPWSTR         lpszFileName; 
        [case(TYMED_ISTREAM)]  IStream        *pstm; 
        [case(TYMED_ISTORAGE)] IStorage       *pstg; 
        [default] ; 
    }; 
    [unique] IUnknown *pUnkForRelease; 
}
STGMEDIUM; 
typedef STGMEDIUM *LPSTGMEDIUM; 
*/
/*
pFormatetc
	[in]
	Pointer to the FORMATETC structure that defines the format, medium, and target device to use when passing the data.
	It is possible to specify more than one medium by using the Boolean OR operator,
	allowing the method to choose the best medium among those specified. 

pmedium
	[out]
	Pointer to the STGMEDIUM structure that indicates the storage medium containing the returned data
	through its tymed member, and the responsibility for releasing the medium
	through the value of its pUnkForRelease member.
	If pUnkForRelease is NULL, the receiver of the medium is responsible for releasing it; otherwise,
	pUnkForRelease points to the IUnknown on the appropriate object so its Release method can be called.
	The medium must be allocated and filled in by IDataObject::GetData.
*/
STDMETHODIMP CDataObject::GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
{
	g_test++;
	if(0){}
	else if(pformatetcIn->cfFormat == CF_TEXT){
		HGLOBAL hglobal;
		char    *p;

//		m_szText[0]++;
		hglobal = GlobalAlloc(GHND, lstrlenA(m_szText) + 1);

		p = (char *)GlobalLock(hglobal);
		lstrcpyA(p, m_szText);
		GlobalUnlock(hglobal);

		pmedium->tymed = TYMED_HGLOBAL;
		pmedium->hGlobal = hglobal;
		pmedium->pUnkForRelease = NULL;
	}
	/*
	else if(pformatetcIn->cfFormat == CF_HDROP){
		HGLOBAL hMem = ::GlobalAlloc(GHND, 1024);
		void* pMem = ::GlobalLock(hMem);
		DROPFILES* df = (DROPFILES*)pMem;
		df->pFiles = sizeof(*df);
		df->pt.x = 0;
		df->pt.y = 0;
		df->fNC = FALSE;
		df->fWide = TRUE;
		wchar_t* p = (wchar_t*)&df[1];
		wcscpy(p, L"D:\\_tmp\\abc.txt"); p += wcslen(p) + 1;
		wcscpy(p, L"D:\\_tmp\\def.txt"); p += wcslen(p) + 1;
		*p = 0;
		::GlobalUnlock(hMem);

		pmedium->tymed = TYMED_HGLOBAL;
		pmedium->hGlobal = hMem;
		pmedium->pUnkForRelease = NULL;
	}
	*/
	else if(pformatetcIn->cfFormat == CF_BITMAP){
		HDC     hdcMem;
		HBITMAP hbmpMem, hbmpMemPrev;
		SIZE    size;
		RECT    rc;

		hdcMem = CreateCompatibleDC(NULL);
		GetTextExtentPoint32A(hdcMem, m_szText, lstrlenA(m_szText), &size);
		hbmpMem = CreateCompatibleBitmap(hdcMem, size.cx, size.cy);
		hbmpMemPrev = (HBITMAP)SelectObject(hdcMem, hbmpMem);

		SetRect(&rc, 0, 0, size.cx, size.cy);
		FillRect(hdcMem, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

		SetBkMode(hdcMem, TRANSPARENT);
		TextOutA(hdcMem, 0, 0, m_szText, lstrlenA(m_szText));
		
		pmedium->tymed = TYMED_GDI;
		pmedium->hBitmap = hbmpMem;
		pmedium->pUnkForRelease = NULL;
		
		SelectObject(hdcMem, hbmpMemPrev);
		DeleteDC(hdcMem);
	}
	else if(pformatetcIn->cfFormat == CClipboard::S_CF_PREFERREDDROPEFFECT()){
		HGLOBAL hglobal = GlobalAlloc(GHND, sizeof(DWORD));
		DWORD* p = (DWORD*)GlobalLock(hglobal);
		*p = DROPEFFECT_COPY;
		GlobalUnlock(hglobal);

		pmedium->tymed = TYMED_HGLOBAL;
		pmedium->hGlobal = hglobal;
		pmedium->pUnkForRelease = NULL;
	}
	else if(pformatetcIn->cfFormat == CClipboard::S_CF_FILEDESCRIPTOR()){
		/*
		typedef struct _FILEGROUPDESCRIPTOR {
			UINT cItems;
			FILEDESCRIPTOR fgd[1];
		}
		FILEGROUPDESCRIPTOR, *LPFILEGROUPDESCRIPTOR;
		*/
		/*
		typedef struct _FILEDESCRIPTOR {
			DWORD dwFlags;
			CLSID clsid;
			SIZEL sizel;				// The width and height of the file icon.
			POINTL pointl;
			DWORD dwFileAttributes;
			FILETIME ftCreationTime;
			FILETIME ftLastAccessTime;
			FILETIME ftLastWriteTime;
			DWORD nFileSizeHigh;
			DWORD nFileSizeLow;
			TCHAR cFileName[MAX_PATH];	// The null-terminated string that contains the name of the file.
		}
		FILEDESCRIPTOR, *LPFILEDESCRIPTOR;
		*/
		HGLOBAL hMem = ::GlobalAlloc(GHND, 1024);
		FILEGROUPDESCRIPTOR* fgd = (FILEGROUPDESCRIPTOR*)::GlobalLock(hMem);
		fgd->cItems = 2;
		fgd->fgd[0].dwFlags = FD_FILESIZE;
		fgd->fgd[0].nFileSizeLow = 10;
		wcscpy(fgd->fgd[0].cFileName, L"A.txt");
		fgd->fgd[1].dwFlags = FD_FILESIZE;
		fgd->fgd[1].nFileSizeLow = 15;
		wcscpy(fgd->fgd[1].cFileName, L"B.txt");
		::GlobalUnlock(hMem);

		pmedium->tymed = TYMED_HGLOBAL;
		pmedium->hGlobal = hMem;
		pmedium->pUnkForRelease = NULL;
	}
	else if(pformatetcIn->cfFormat == CClipboard::S_CF_FILECONTENTS()){
		if(1){
			HGLOBAL hMem = ::GlobalAlloc(GHND, 1024);
			void* p = ::GlobalLock(hMem);
			memcpy(p, "12345678901234567890", 20);
			::GlobalUnlock(hMem);

			pmedium->tymed = TYMED_HGLOBAL;
			pmedium->hGlobal = hMem;
			pmedium->pUnkForRelease = NULL;
		}
		else{
//			pmedium->tymed = TYMED_ISTREAM;
//			pmedium->pstm = this;
//			pmedium->pUnkForRelease = NULL;
		}
	}
	else{
		return E_FAIL;
	}
	
	return S_OK;
}

/*
This method is called by a data consumer to obtain data from a source data object.
This method differs from the GetData method in that the caller must allocate and free the specified storage medium.
*/
STDMETHODIMP CDataObject::GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium)
{
	return E_NOTIMPL;
#if 1
	if (pformatetc->cfFormat == CF_TEXT) {
		ULONG uWritten;
		pmedium->pstm->Write(m_szText, lstrlenA(m_szText) + 1, &uWritten);
		return S_OK;
	}
	else if(pformatetc->cfFormat == CClipboard::S_CF_FILECONTENTS()){
		if(pformatetc->lindex == 0){
			ULONG uWritten;
			pmedium->pstm->Write("1234567890", 10, &uWritten);
			return S_OK;
		}
		else if(pformatetc->lindex == 1){
			ULONG uWritten;
			pmedium->pstm->Write("123456789012345", 15, &uWritten);
			return S_OK;
		}
		else{
			return E_FAIL;
		}
	}
	else{
		return E_FAIL;
	}
#elif 1
	// ###仮
	return E_NOTIMPL;
#else
	if (pformatetc->cfFormat == CF_TEXT) {
		ULONG uWritten;
		pmedium->pstm->Write(m_szText, lstrlenA(m_szText) + 1, &uWritten);
	}
	else
		return E_FAIL;

	return S_OK;
#endif
}

STDMETHODIMP CDataObject::QueryGetData(FORMATETC *pformatetc)
{
	g_test++;
	switch(pformatetc->cfFormat){
	case CF_TEXT:
	case CF_BITMAP:
//	case CF_HDROP:
		return S_OK;
	default:
		if(pformatetc->cfFormat == CClipboard::S_CF_PREFERREDDROPEFFECT()){
			return S_OK;
		}
		if(pformatetc->cfFormat == CClipboard::S_CF_FILEDESCRIPTOR()){
			return S_OK;
		}
		if(pformatetc->cfFormat == CClipboard::S_CF_FILECONTENTS()){
			return S_OK;
		}
	}

	return E_NOTIMPL;
}

// IEnumFORMATETC::Next
STDMETHODIMP CDataObject::Next(
	ULONG		celt,			// [in] Number of elements being requested. 
	FORMATETC*	rgelt,			// [out] Array of size celt (or larger) of the elements of interest.
								//       The type of this parameter depends on the item being enumerated.
	ULONG*		pceltFetched	// [in, out] Pointer to the number of elements actually supplied in rgelt.
								//           The caller can pass in NULL if celt is 1.
)
{
	/*
	typedef struct tagFORMATETC 
	{ 
		CLIPFORMAT      cfFormat; 
		DVTARGETDEVICE  *ptd; 
		DWORD           dwAspect; 
		LONG            lindex;		// Part of the aspect when the data must be split across page boundaries.
									// The most common value is -1, which identifies all of the data. 
		DWORD           tymed; 
	}FORMATETC, *LPFORMATETC;
	*/
	FORMATETC formatetc[] = {
		{
			CF_TEXT,
			NULL,
			DVASPECT_CONTENT,
			-1,
//			TYMED_HGLOBAL// | TYMED_ISTREAM
			TYMED_HGLOBAL //TYMED_ISTREAM
		},
		{
			CF_BITMAP,
			NULL,
			DVASPECT_CONTENT,
			-1,
			TYMED_GDI
		},
		/*
		{
			CF_HDROP,
			NULL,
			DVASPECT_CONTENT,
			-1,
			TYMED_HGLOBAL
		},
		*/
		{
			CClipboard::S_CF_PREFERREDDROPEFFECT(),
			NULL,
			DVASPECT_CONTENT,
			-1,
			TYMED_HGLOBAL
		},
		{
			CClipboard::S_CF_FILECONTENTS(),
			NULL,
			DVASPECT_CONTENT,
			-1,
			TYMED_HGLOBAL// | TYMED_ISTREAM
		},
		{
			CClipboard::S_CF_FILEDESCRIPTOR(),
			NULL,
			DVASPECT_CONTENT,
			-1,
			TYMED_HGLOBAL
		},
	};

	if (m_uEnumCount >= _countof(formatetc))
		return S_FALSE;
	
	*rgelt = formatetc[m_uEnumCount];

	if (pceltFetched != NULL)
		*pceltFetched = 1;
	
	m_uEnumCount++;

	return S_OK;
}

STDMETHODIMP CDataObject::GetCanonicalFormatEtc(FORMATETC *pformatectIn, FORMATETC *pformatetcOut)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataObject::SetData(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease)
{
	/*
		CFSTR_PASTESUCCEEDED

		This format identifier is used by the target to inform the data object,
		through its IDataObject::SetData method, that a delete-on-paste operation succeeded.
		The data is an STGMEDIUM structure that contains a global memory object.
		The structure's hGlobal member points to a DWORD containing a DROPEFFECT value.
		This format is used to notify the data object that it should complete
		the cut operation and delete the original data, if necessary.
		For more information, see Delete-on-Paste Operations.

		-- Delete-on-Paste Operations --
		The basic procedure for a delete-on-paste operation is as follows:

		1. The source marks the screen display of the selected data.
		2. The source creates a data object.
		   It indicates a cut operation by adding the CFSTR_PREFERREDDROPEFFECT format with a data value of DROPEFFECT_MOVE.
		3. The source places the data object on the Clipboard using OleSetClipboard.
		4. The target retrieves the data object from the Clipboard using OleGetClipboard.

		5. The target extracts the CFSTR_PREFERREDDROPEFFECT data.
		   If it is set to only DROPEFFECT_MOVE, the target can either do an optimized move or simply copy the data.

		6. If the target does not do an optimized move, it calls the IDataObject::SetData method
		   with the CFSTR_PERFORMEDDROPEFFECT format set to DROPEFFECT_MOVE.
		7. When the paste is complete, the target calls the IDataObject::SetData method
		   with the CFSTR_PASTESUCCEEDED format set to DROPEFFECT_MOVE.
		8. When the source's IDataObject::SetData method is called
		   with the CFSTR_PASTESUCCEEDED format set to DROPEFFECT_MOVE,
		   it must check to see if it also received the CFSTR_PERFORMEDDROPEFFECT format set to DROPEFFECT_MOVE.
		   If both formats are sent by the target, the source will have to delete the data.

		   If only the CFSTR_PASTESUCCEEDED format is received,
		   the source can simply remove the data from its display.

		   If the transfer fails, the source updates the display to its original appearance.
	*/
	if(pformatetc->cfFormat == CClipboard::S_CF_PERFORMEDDROPEFFECT() && pmedium->tymed == TYMED_HGLOBAL){
		if(0)return E_NOTIMPL;

		// (2)
		// ※切り取り元のファイルが削除され、切り取り先に移動される前に、ここが呼ばれるっぽい //
		// ※コピーが完了する前に、ここが呼ばれるっぽい //
		g_test++;
		DWORD dwEffect = *(DWORD*)::GlobalLock(pmedium->hGlobal);
		::GlobalUnlock(pmedium->hGlobal);
		if(fRelease){
			::ReleaseStgMedium(pmedium);
		}
		return S_OK;
	}
	else if(pformatetc->cfFormat == CClipboard::S_CF_PASTESUCCEEDED()){
		g_paste_count++;
		{
			wchar_t buf[256];
			swprintf(buf, L"paste:%d", g_paste_count);
			::SetWindowText(g_hwndMain, buf);
		}
		DWORD dwEffect = *(DWORD*)::GlobalLock(pmedium->hGlobal);
		::GlobalUnlock(pmedium->hGlobal);
		if(fRelease){
			::ReleaseStgMedium(pmedium);
		}
		return S_OK;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_TARGETCLSID)){
		// (1)
		// コピー先 //
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_LOGICALPERFORMEDDROPEFFECT)){
		// (3)
		g_test++;
	}
	else if(pformatetc->cfFormat == CClipboard::S_CF_FILECONTENTS()){
		g_test++;
	}
	else if(pformatetc->cfFormat == CClipboard::S_CF_FILEDESCRIPTOR()){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILEDESCRIPTOR)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_SHELLIDLIST)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_SHELLIDLISTOFFSET)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_NETRESOURCES)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILEDESCRIPTORA)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILECONTENTS)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILENAMEA)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILENAMEW)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_PRINTERGROUP)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILENAMEMAPA)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_FILENAMEMAPW)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_SHELLURL)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_INETURLA)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_INETURLW)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_PERFORMEDDROPEFFECT)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_PASTESUCCEEDED)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_INDRAGLOOP)){
		g_test++;
	}
//	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_DRAGCONTEXT)){
//		g_test++;
//	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_MOUNTEDVOLUME)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_PERSISTEDDATAOBJECT)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_AUTOPLAY_SHELLIDLISTS)){
		g_test++;
	}
	else if(pformatetc->cfFormat == RegisterClipboardFormat(CFSTR_UNTRUSTEDDRAGDROP)){
		g_test++;
	}

	return E_NOTIMPL;
}

STDMETHODIMP CDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc)
{
	if (dwDirection == DATADIR_GET)
		return QueryInterface(IID_PPV_ARGS(ppenumFormatEtc));
	else
		return E_NOTIMPL;
}


STDMETHODIMP CDataObject::DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataObject::DUnadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataObject::EnumDAdvise(IEnumSTATDATA **ppenumAdvise)
{
	return E_NOTIMPL;
}


STDMETHODIMP CDataObject::Skip(ULONG celt)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataObject::Reset(VOID)
{
	m_uEnumCount = 0;

	return S_OK;
}

STDMETHODIMP CDataObject::Clone(IEnumFORMATETC **ppenum)
{
	return E_NOTIMPL;
}
