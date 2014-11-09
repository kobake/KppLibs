#include "_required.h"
#include "Desktop.h"
#include "../Cast.h"
#include <windows.h>

math::CRect Desktop::getWorkArea() const
{
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA,NULL,&rc,NULL);
	return RECT2Rect(rc);
}
