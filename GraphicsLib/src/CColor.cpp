#include "_required.h"
#include "CColor.h"
#include <windows.h>

CColor& CColor::operator *= (double n)
{
	r=(byte)(r*n);
	g=(byte)(g*n);
	b=(byte)(b*n);
	return *this;
}
CColor& CColor::operator /= (double n)
{
	r=(byte)(r/n);
	g=(byte)(g/n);
	b=(byte)(b/n);
	return *this;
}

RGBQUAD CColor::win_color2() const
{
	return *((RGBQUAD*)&w_color);
}


