#include <BaseLib.h>
#include "CDcGraphics.h"
// #include <Cast.h> //WinSystemLib
#include <WinSystemLib.h>
#include "CDib.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �������ƏI��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �P����HDC�����b�v����
CDcGraphics::DcImp_Wrap::DcImp_Wrap(HDC hdc, const math::CRect& rc)
{
	_Setup(hdc);
	_Setup(rc);
}
CDcGraphics::DcImp_Wrap::~DcImp_Wrap()
{
	//�������Ȃ�
}

// WM_PAINT�����̂��߂�HDC��񋟂���
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

// ������DC
CDcGraphics::DcImp_Memory::DcImp_Memory(const math::CBoxSize& cBoxSize)
{
	// �X�N���[��DC�擾
	HDC hdcModel=GetDC(NULL);

	// �r�b�g�}�b�v�쐬�A
	m_hbmp = CreateCompatibleBitmap(hdcModel, cBoxSize.w, cBoxSize.h);

	// ������DC�쐬�A�r�b�g�}�b�v�I��
	HDC hdc = CreateCompatibleDC(hdcModel);
	m_hbmpOrg = (HBITMAP)SelectObject(hdc, m_hbmp);

	// �X�N���[��DC���
	ReleaseDC(NULL, hdcModel);

	// �����h��Ԃ�
	::FillRect(hdc, &Rect2RECT(math::CRect(cBoxSize)), (HBRUSH)GetStockObject(BLACK_BRUSH));

	// �����o�ϐ��ݒ�
	_Setup(hdc);
	_Setup(math::CRect(cBoxSize));
}

CDcGraphics::DcImp_Memory::~DcImp_Memory()
{
	::SelectObject(_GetHDC(), m_hbmpOrg);
	::DeleteObject(m_hbmp);
	::DeleteDC(_GetHDC());
}

//���X�̏����擾���Ă���
void CDcGraphics::_AfterInit()
{
	m_hdc = m_pcImp->_GetHDC();
	m_rc = m_pcImp->_GetRect();

	//���_���ł��� (���Ȃ��Ƃ�WM_PAINT�ł͎g���Ȃ�)
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
// �}�`�`��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//####�u���V�̃v�[��������Č������������Ƃ���

void CDcGraphics::FillRect(const math::CRect& rc, COLORREF color)
{
	//�ꎞ�I�Ƀu���V���쐬
	CBrush br(color);

	::FillRect(m_hdc, &Rect2RECT(rc), br.GetHBRUSH());
}

void CDcGraphics::DrawRect(const math::CRect& rc, COLORREF color)
{
	//�ꎞ�I�ɓ����u���V����ѐF�y�����쐬�A�ꎞ�I�ɂ�����I��
	CBrush br;
	CPen pen(color);
	CDcSelector sel(m_hdc, br);
	CDcSelector sel2(m_hdc, pen);

	::Rectangle(m_hdc, rc.Left(), rc.Top(), rc.Right(), rc.Bottom());
}

void CDcGraphics::DrawLine(const math::CPoint& ptStart, const math::CPoint& ptEnd, COLORREF color)
{
	//�ꎞ�I�ɐF�y�����쐬�A�ꎞ�I�ɂ����I��
	CPen pen(color);
	CDcSelector sel(m_hdc, pen);

	::MoveToEx(m_hdc, ptStart.x, ptStart.y, NULL);
	::LineTo(m_hdc, ptEnd.x, ptEnd.y);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// �e�L�X�g�`��
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CDcGraphics::SetTextFont(const wchar_t* szFace, const CPixel& cHeight)
{
	delete m_pcFont;
	m_pcFont = new CFont(szFace, cHeight);
	::SelectObject(m_hdc, m_pcFont->GetHFONT()); //��HDC�����݂��Ă���ԁAm_pcFont�������邱�Ƃ������̂ŁA�Â��t�H���g�͕ێ����Ă����K�v���Ȃ�
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
// �C���[�W�`��
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
