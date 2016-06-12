#include <BaseLib.h>
#include "CAccelerator.h"

CAccelerator::CAccelerator(int nId)
{
	m_hAccel = ::LoadAccelerators(BaseLib::GetInstance(), MAKEINTRESOURCE(nId));
}

CAccelerator::~CAccelerator()
{
	::DeleteObject(m_hAccel);
}
