#include <BaseLib.h>
#include "CDcGraphics.h"
// #include <Cast.h> //WinSystemLib
#include <WinSystemLib.h>
#include "CDib.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 単純にHDCをラップする
CDcGraphics::DcImp_Wrap::DcImp_Wrap(HDC hdc, const math::CRect& rc)
{
	_Setup(hdc);
	_Setup(rc);
}
CDcGraphics::DcImp_Wrap::~DcImp_Wrap()
{
	//何もしない
}

// WM_PAINT処理のためのHDCを提供する
CDcGraphics::DcImp_Message::DcImp_Message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
: m_hwnd(hwnd)
{
	HDC hdc = ::BeginPaint(m_hwnd, &m_ps);
	_Setup(hdc);
	RECT rc = {0};
	::GetClientRect(hwnd, &rc);
	_Setup(RECT2Rect(rc));
}
CDcGraphics::DcImp_Message::~DcImp_Message()
{
	::EndPaint(m_hwnd, &m_ps);
}

// メモリDC
CDcGraphics::DcImp_Memory::DcImp_Memory(const math::CBoxSize& cBoxSize)
{
	// スクリーンDC取得
	HDC hdcModel=GetDC(NULL);

	// ビットマップ作成、
	m_hbmp = CreateCompatibleBitmap(hdcModel, cBoxSize.w, cBoxSize.h);

	// メモリDC作成、ビットマップ選択
	HDC hdc = CreateCompatibleDC(hdcModel);
	m_hbmpOrg = (HBITMAP)SelectObject(hdc, m_hbmp);

	// スクリーンDC解放
	ReleaseDC(NULL, hdcModel);

	// 黒く塗りつぶす
	::FillRect(hdc, &Rect2RECT(math::CRect(cBoxSize)), (HBRUSH)GetStockObject(BLACK_BRUSH));

	// メンバ変数設定
	_Setup(hdc);
	_Setup(math::CRect(cBoxSize));
}

CDcGraphics::DcImp_Memory::~DcImp_Memory()
{
	::SelectObject(_GetHDC(), m_hbmpOrg);
	::DeleteObject(m_hbmp);
	::DeleteDC(_GetHDC());
}

//諸々の情報を取得しておく
void CDcGraphics::_AfterInit()
{
	m_hdc = m_pcImp->_GetHDC();
	m_rc = m_pcImp->_GetRect();

	//↓ダメでした (少なくともWM_PAINTでは使えない)
	/*
	HBITMAP hbmp = (HBITMAP)::GetCurrentObject(m_hdc, OBJ_BITMAP);
	BITMAP bmp;
	int w = 0;
	int h = 0;
	int n = ::GetObject(hbmp, 0, NULL);
	int n1 = sizeof(BITMAP);
	int n2 = sizeof(DIBSECTION);
	if(::GetObject(hbmp, n, &bmp)){
		w = bmp.bmWidth;
		h = bmp.bmHeight;
	}
	m_rc.Assign(0, 0, w, h);
	*/
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 図形描画
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//####ブラシのプールを作って効率化したいところ

void CDcGraphics::FillRect(const math::CRect& rc, COLORREF color)
{
	//一時的にブラシを作成
	CBrush br(color);

	::FillRect(m_hdc, &Rect2RECT(rc), br.GetHBRUSH());
}

void CDcGraphics::DrawRect(const math::CRect& rc, COLORREF color)
{
	//一時的に透明ブラシおよび色ペンを作成、一時的にそれらを選択
	CBrush br;
	CPen pen(color);
	CDcSelector sel(m_hdc, br);
	CDcSelector sel2(m_hdc, pen);

	::Rectangle(m_hdc, rc.Left(), rc.Top(), rc.Right(), rc.Bottom());
}

void CDcGraphics::DrawLine(const math::CPoint& ptStart, const math::CPoint& ptEnd, COLORREF color)
{
	//一時的に色ペンを作成、一時的にそれを選択
	CPen pen(color);
	CDcSelector sel(m_hdc, pen);

	::MoveToEx(m_hdc, ptStart.x, ptStart.y, NULL);
	::LineTo(m_hdc, ptEnd.x, ptEnd.y);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// テキスト描画
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CDcGraphics::SetTextFont(const wchar_t* szFace, const CPixel& cHeight)
{
	delete m_pcFont;
	m_pcFont = new CFont(szFace, cHeight);
	::SelectObject(m_hdc, m_pcFont->GetHFONT()); //※HDCが存在している間、m_pcFontが消えることも無いので、古いフォントは保持しておく必要もない
}

void CDcGraphics::SetTextColor(COLORREF color)
{
	::SetTextColor(m_hdc, color);
	::SetBkMode(m_hdc, TRANSPARENT);
}

void CDcGraphics::DrawText(const wchar_t* szText, const math::CPoint& ptPos)
{
	::TextOut(m_hdc, ptPos.x, ptPos.y, szText, (int)wcslen(szText));
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// イメージ描画
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CDcGraphics::DrawImage(const math::CRect& rcDst, const CDib& cDib, const math::CRect& rcSrc)
{
	::StretchDIBits(
		this->m_pcImp->_GetHDC(),
		rcDst.Left(),
		rcDst.Top(),
		rcDst.Width(),
		rcDst.Height(),
		rcSrc.Left(),
		rcSrc.Top(),
		rcSrc.Width(),
		rcSrc.Height(),
		cDib.GetBits(),
		cDib.GetBitmapInfo(),
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

void CDcGraphics::DrawImage(const math::CRect& rcDst, HDC hdc, const math::CRect& rcSrc)
{
	::StretchBlt(
		this->m_pcImp->_GetHDC(),
		rcDst.Left(),
		rcDst.Top(),
		rcDst.Width(),
		rcDst.Height(),
		hdc,
		rcSrc.Left(),
		rcSrc.Top(),
		rcSrc.Width(),
		rcSrc.Height(),
		SRCCOPY
	);
}
