#pragma once

#include <windows.h> //LOGFONT
#include "../../MathLib/MathLib.h" //CBoxSize
#include <string>
#include "CGdiObj.h"

class CPixel{
public:
	explicit CPixel(int n) : m_nValue(n) { }
	int Get() const{ return m_nValue; }
private:
	int m_nValue;
};

class CPointSize{
public:
	explicit CPointSize(int n) : m_nValue(n) { }
	int Get() const{ return m_nValue; }
	//�s�N�Z���ɕϊ�
	operator CPixel () const
	{
		HDC hdc = ::GetDC(NULL);
		int logpix = ::GetDeviceCaps(hdc, LOGPIXELSY);
		::ReleaseDC(NULL,hdc);

		int pixel = -MulDiv(m_nValue, logpix, 72);
		return CPixel(pixel);
	}
private:
	int m_nValue;
};

namespace gr{

class CFont : public CGdiObj{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFont(const wchar_t* szFace, const CPixel& cHeight);
	virtual ~CFont();

	//�C���^�[�t�F�[�X
	HGDIOBJ GetHGDIOBJ() const{ return m_hFont; }
	HFONT GetHFONT() const{ return m_hFont; }
private:
	HFONT m_hFont;
};

} // namespace gr

using namespace gr;
