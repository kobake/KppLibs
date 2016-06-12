#include <BaseLib.h>
#include "CFont.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CFont::CFont(const wchar_t* szFace, const CPixel& cHeight)
{
	//LOGFONT構築
	LOGFONT lf = {0};
	lf.lfHeight = cHeight.Get();
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = SHIFTJIS_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = FIXED_PITCH;
	wcscpy_s(lf.lfFaceName, _countof(lf.lfFaceName), szFace);

	//フォント作成
	m_hFont = CreateFontIndirect(&lf);
}

CFont::~CFont()
{
	DeleteObject(m_hFont);
}



